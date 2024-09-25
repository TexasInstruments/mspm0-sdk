import click
import string

@click.command()
@click.option('-f','--binfile',help="input binary file")
@click.option('-o','--outfile',help="output file")
@click.option('-s','--startaddr',help="start Address")
def bin_to_titxt(binfile, outfile, startaddr): 
    print("converting {} to {}".format(binfile, outfile))
    f = open(binfile,'rb')
    o = open(outfile, 'w')

    o.write("@{}\n".format(startaddr))
    x = f.read(1)
    i = 0
    while x: 
        a = int.from_bytes(x, "little")
        o.write("{:02x}".format(a).upper())
        if i < 15: 
            o.write(" ")
        else :
            o.write("\n")
        i = (i + 1) % 16
        x = f.read(1)


    # TODO: check if not on a newline
    o.write('q')

    f.close()
    o.close()

if __name__ == "__main__":
    bin_to_titxt()