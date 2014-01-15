
input_filename = "Run10.dat"

f = open( input_filename, "rb")

cnt_bytes = -1                        
cnt_words = 0


def do_read( f ):
    global cnt_bytes
    global cnt_words

    byte = f.read(1)

    cnt_bytes += 1
    if cnt_bytes == 4:
        cnt_bytes = 0
        cnt_words += 1
        
    return byte
    

try:
    byte = do_read( f )

    li_channels = []

                
    while byte != "" and cnt_words < 10000:

        # Do stuff with byte.
        if (cnt_words == 1030) or (cnt_words == 1543) or (cnt_words == 2056) or (cnt_words == 2569):
            if cnt_bytes == 3:
                if byte == "R":
                    break
                else:
                    li_channels.append( byte )

        # And read the next one 
        byte = do_read( f )
        
finally:
    f.close()

print li_channels
