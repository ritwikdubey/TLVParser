import binascii
# Script to convert the example mentioned in the pdf to a bin file. 
# Acts as a second test case to validate TLVHelper
with open('sample.binimage') as f, open('sample.bin', 'wb') as fout:
    for line in f:
        fout.write(
            binascii.unhexlify(''.join(line.split()))
        )
