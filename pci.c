#include <pci.h>

#include <io.h>
#include <log.h>

#define PCI_CONFIG  0xCF8
#define PCI_DATA    0xCFC

uint32_t
pci_read(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset) {
    uint32_t reg = 0x80000000;

    reg |= (bus & 0xFF) << 16;
    reg |= (device & 0x1F) << 11;
    reg |= (function & 0x7) << 8;
    reg |= (offset & 0xFF) & 0xFC;

    outportl(PCI_CONFIG, reg );

    return inportl(PCI_DATA);
}

void
pci_write(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset, uint32_t data) {
    uint32_t reg = 0x80000000;

    reg |= (bus & 0xFF) << 16;
    reg |= (device & 0x1F) << 11;
    reg |= (function & 0x7) << 8;
    reg |= offset & 0xFC;

    outportl(PCI_CONFIG, reg );
    outportl(PCI_DATA, data);
}

uint8_t
pci_find(uint32_t vendor, uint32_t device, uint8_t* bus,  uint8_t* dev, uint8_t* function) {
  uint32_t vend_dev, b, d, f, my_vend_dev;

  my_vend_dev = ( vendor & 0xFFFF ) | (device << 16);

  for(b = 0; b < 256; b++)
    for(d = 0; d < 32; d++)
      for(f = 0; f < 8; f++){
        vend_dev = pci_read(b,d,f,PCI_VENDOR_DEVICE);
        if(vend_dev == my_vend_dev) {
          *bus = b;
          *dev = d;
          *function = f;
          return 1;
        }
      }
  return 0;
}

uint8_t
pci_find_class(int8_t class, uint8_t subclass, uint8_t* bus,  uint8_t* dev, uint8_t* function) {
  uint32_t b, d, f;
  uint16_t my_cls_sub, cls_sub;

  my_cls_sub = class | (subclass << 8);

  for(b = 0; b < 256; b++)
    for(d = 0; d < 32; d++)
      for(f = 0; f < 8; f++){
        cls_sub = pci_read(b,d,f,PCI_CLASS_SUBCLASS) >> 16;
        if(cls_sub == my_cls_sub) {
          *bus = b;
          *dev = d;
          *function = f;
          return 1;
        }
      }
  return 0;
}

void pci_test() {
    uint32_t vend_dev, b, d, f, devices = 0;
    klogf(LOG_INFO, "PCI devices:\n");
    for(b = 0; b < 256; b++)
        for(d = 0; d < 32; d++)
            for(f = 0; f < 8; f++) {
                vend_dev = pci_read(b,d,f,PCI_VENDOR_DEVICE);
                if(vend_dev != 0xFFFFFFFF) {
                    unsigned short vendor = vend_dev & 0xffff;
                    unsigned short device = (vend_dev >> 16) & 0xffff;

                    klogf(LOG_INFO, " * [%d:%d.%d] %x:%x\n", b, d, f, vendor, device);
                    
                    if (vendor == 0x8086 && device == 0x2415) {
                        klogf(LOG_INFO, "=> found 82801 AA AC'97 sound card.\n");
                    }

                    devices++;
                }
            }

    klogf(LOG_INFO, "PCI scan: found %d devices.\n", devices);
}

