This program reads two long integers from an input file and manually performs an addition operation between the two numbers. Each long integer read from the input file is placed into a doubly linked list. Its digits are split up into groups of 5 and placed in each link. It is possible for the front link of the list to have less than 5 digits. From right to left, the first link in List 1 is added to the first link in List 2. This sum is placed in the corresponding link in List 3. A carry number may need to be moved over to the next link if the sum of the two links exceeds 99,999.

This program was written for the Advanced C Programming course with UCSC Extension.

Comments and suggestions are welcome.