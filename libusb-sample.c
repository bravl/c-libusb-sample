#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hidapi.h"

#define MAX_STR 255
#define OPS_VID 0x270f
#define OPS_UID 0x3039

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[65];
	memset(buf,0,65);
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	// Initialize the hidapi library
	res = hid_init();
	struct hid_device_info *devs, *cur_dev;

	devs = hid_enumerate(OPS_VID,OPS_UID);
	cur_dev = devs;

	while(cur_dev) {
		wprintf(L"%ls %ls\n", cur_dev->manufacturer_string, cur_dev->product_string);
		if (cur_dev->manufacturer_string != NULL && cur_dev->product_string != NULL) {
			handle = hid_open_path(cur_dev->path);
			break;
		}
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);
	if (!handle) return -1;

	unsigned char buf2[8];
	memset(buf2,0,8);
	sleep(1);
	strcpy(&buf[0],"5678lala");
	int val = hid_write(handle, buf, 8);
	if (val < 0) {
		printf("Failed to write to HID\n");
	}

	val = hid_read(handle,buf2,8);
	for (int i =0; i < val; i++) {
		wprintf(L"%x - ", buf2[i]);
	}
	val = hid_read(handle,buf2,8);
	wprintf(L"Val: %d\n",val);
	if (val < 0) {
		printf("Failed to write to HID\n");
	}
	for (int i =0; i < val; i++) {
		wprintf(L"%x - ", buf2[i]);
	}
	wprintf(L"/n");

	sleep(1);

	return 0;
}
