# Computes and prints the sum of entered ints;
# Input is terminated by 0

       ld null  # Set sum to 0
       st sum
loop   get
       jz fertig
       add sum
       st sum
       j loop

fertig ld sum
       put
       halt

null const 0
sum  const
