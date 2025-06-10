# write binary data to gummyB_data.bin (NOT .c)
with open("gummyB.pcm", "rb") as f_in:
    data = f_in.read()

with open("gummyB_data.bin", "wb") as f_out:
    f_out.write(data)
