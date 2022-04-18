/*
 * the toyota char device driver.
 */

#include <linux/module.h>
#include <linux/kernel.h>  /* printk() */
#include <linux/slab.h>    /* for kmalloc() */
#include <linux/version.h> /* for kmalloc() */
#include <linux/fs.h>      /* everything... */
#include <linux/file.h>    /* everything... */
#include <linux/errno.h>   /* error codes */
#include <linux/types.h>   /* size_t */
#include <linux/fcntl.h>   /* O_ACCMODE */
#include <linux/kmod.h>    /* for request_module */
#include <linux/init.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 12)
#include <linux/sched/signal.h>
#else
#include <linux/signal.h>
#endif
#include <linux/sched.h>
#include <linux/uaccess.h>

#include "toyota.h" /* local definitions */

MODULE_AUTHOR("Jason Kuphaldt"); /* change this line to your name */
MODULE_LICENSE("GPL");

static int toyota_open(struct inode *inode, struct file *filp);
static int toyota_release(struct inode *inode, struct file *filp);
static ssize_t toyota_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t toyota_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static char *removeDuplicateLetters(char *s, size_t slen);

int device;
int majorDevNum;
char *kbuf;
size_t kbufSize;

/* The different file operations.
 * Any member of this structure which we don't explicitly assign will be initialized to NULL by gcc. */
static struct file_operations toyota_fops = {
    .owner = THIS_MODULE,
    .read = toyota_read,
    .write = toyota_write,
    .open = toyota_open,
    .release = toyota_release,
};

/*
 * open. if successful, return 0.
 */

static int toyota_open(struct inode *inode, struct file *filp)
{
    //get the minor number
    int minorNum = NUM(inode->i_rdev);
    if (minorNum >= 4 || minorNum < 0)
    {
        return -ENODEV;
    }

    //put minor number in global variable
    device = minorNum;

    /* increment the use count. */
    try_module_get(THIS_MODULE);
    return 0; /* success */
}

/*
 * close. if successful, return 0.
 */

static int toyota_release(struct inode *inode, struct file *filp)
{

    /* decrement the use count. */
    module_put(THIS_MODULE);
    return 0;
}

/* write the string to internal buffer for minor number 0;
 * pretends to ignore writes (like /dev/null) for minor numbers 1,2;
 * kill the calling process for minor number 3;
 * we assume applications will access our device sequentially, i.e., they do not access multiple devices concurrently.
 * if successful, return count - user wants to write "count" bytes into this device.
 */
static ssize_t toyota_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
    kbuf = kmalloc(count, GFP_KERNEL);
    memset(kbuf, 0, count);
    kbufSize = 0;

    if (device == 0)
    {
        // copy to buffer if minor device number is 0
        if (copy_from_user(kbuf, buf, count) != 0)
        {
            kfree(kbuf);
            return -EACCES;
        }
        kbufSize = count;
    }
    else if (device == 3)
    {
        // if minor device number is 3, kill process
        kill_pid(task_pid(current), SIGTERM, 1);
    }
    
    // just return if minor device number is 1,2
    return count;
}

//removes the duplicate letters and keep lexicograghical order
static char *removeDuplicateLetters(char *s, size_t slen)
{
    size_t i, outlen = 0;
    //keeps track of the amount of occurances of a letter in the string
    int alphabet[26] = {0};
    //keeps track of if a letter is in the stack
    int inStack[26] = {0};

    //make the output buffer (stack) using kmalloc
    char *out = (char *)kmalloc(sizeof(char) * (slen + 1), GFP_KERNEL);
    memset(out, 0, sizeof(char) * (slen + 1));

    //count the occurance of each letter in the string
    for (i = 0; i < slen; i++) {
        alphabet[s[i] - 'a']++;
    }

    for (i = 0; i < slen; i++)
    {
        //if the current letter is in the stack, skip it and decrement the occurance
        if (inStack[s[i] - 'a'])
        {
            alphabet[s[i] - 'a']--;
            continue;
        }
        //if the current letter is less than the stack's head and we can add the letter
        //from the head later (i.e. there is a duplicate down the road), remove the head.
        //keep doing this until this is false.
        while (outlen > 0 && s[i] < out[outlen - 1] && alphabet[out[outlen - 1] - 'a'] > 0)
        {
            inStack[out[outlen - 1] - 'a'] = 0;
            outlen--;
        }

        //Add the letter to the stack
        out[outlen++] = s[i];
        //decrement the occurance of that letter
        alphabet[s[i] - 'a']--;
        //set the inStack of that letter to 1 (true)
        inStack[s[i] - 'a'] = 1;
    }

    //end string with a terminator character
    out[outlen] = '\0';

    return out;
}

/* when read, we do not care the device minor number,
 * we process whatever is in the internal buffer, and return the processed string (maybe multiple times, as a stream) to user.
 * we assume applications will access our device sequentially, i.e., they do not access multiple devices concurrently.
 * if successful, return count - user wants to read "count" bytes from this device.
 */
static ssize_t toyota_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
    int i;
    size_t len;
    char *out = (char *)kmalloc(count, GFP_KERNEL);
    char *result = removeDuplicateLetters(kbuf, kbufSize);
    
    out[0] = '\0';
    kfree(kbuf);

    len = strlen(result);

    //creates the stream to be returned to the user
    if (len > 0) {
        for (i = 0; i < (int)(count / len); i++) {
            strcat(out, result);
        }
        if ((count % len) > 0) {
            strncat(out, result, (count % len));
        }
    }

    len = strlen(out);

    //returns the stream
    if(copy_to_user(buf, out, len) != 0) {
		kfree(out);
		return -EACCES;
	}

    kfree(out);

    return len;
}

/*
 * module initialization. if successful, return 0.
 */

static int __init toyota_init(void)
{
    /*
     * register your major, and accept a dynamic number.
     */
    majorDevNum = register_chrdev(0, "toyota", &toyota_fops);
    return 0;
}

/*
 * module exit. if successful, does not return anything.
 */

static void __exit toyota_exit(void)
{
    /* reverse the effect of register_chrdev(). */
    unregister_chrdev(majorDevNum, "toyota");
}

module_init(toyota_init);
module_exit(toyota_exit);

/* vim: set ts=4: */
