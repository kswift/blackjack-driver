/*
 * Kevin Swift
 * csc352, Spring20
 *
 * card_driver.c ; this file represents the driver for the card deck
 *		   inside of blackjack.c. Yields random card values from
 *		   our blackjack deck.
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/uaccess.h>

/*
* This function returns a random byte 0 - 51 to represent the card in our
* card deck of ints.
*/
unsigned char get_random_byte(int max){
	unsigned char c;
	get_random_bytes(&c, 1);
	return c % max;
}

/*
 * cards_read is the function called when a process calls read() on
 * /dev/blackjack.  It writes a rnadom byte to the buffer passed in the
 * read() call.
 */

static ssize_t cards_read(struct file * file, char * buf,
			  size_t count, loff_t *ppos)
{
	char *cards_str = "Hello, world!\n";
	int len = strlen(cards_str); /* Don't include the null byte. */
	/*
	 * We only support reading the whole string at once.
	 */
	if (count < len)
		return -EINVAL;
	/*
	 * If file position is non-zero, then assume the string has
	 * been read and indicate there is no more data to be read.
	 */
	if (*ppos != 0)
		return 0;
	/*
	 * Besides copying the string to the user provided buffer,
	 * this function also checks that the user has permission to
	 * write to the buffer, that it is mapped, etc.
	 */
	if (copy_to_user(buf, cards_str, len))
		return -EINVAL;
	/*
	 * Tell the user how much data we wrote.
	 */
	*ppos = len;

	return len;
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations cards_fops = {
	.owner		= THIS_MODULE,
	.read		= cards_read,
	.llseek		= default_llseek,
};

static struct miscdevice cards_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/cards.
	 */
	"cards",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&cards_fops
};

static int __init
cards_init(void)
{
	int ret;

	/*
	 * Create the "hello" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/hello device using
	 * the default rules.
	 */
	ret = misc_register(&cards_dev);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"Card Driver\" misc device\n");

	return ret;
}

module_init(cards_init);

static void __exit
cards_exit(void)
{
	misc_deregister(&cards_dev);
}

module_exit(cards_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valerie Henson <val@nmt.edu>");
MODULE_DESCRIPTION("\"Card Driver\" minimal module");
MODULE_VERSION("dev");
