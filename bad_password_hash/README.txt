This is public domain software, written by Karl Magdsick.
It provides poor security and is not suitable for any purpose.  Its use is discouraged.

bad_password_hash implements a 9-digit base-94 generalized feedback shift register for hashing
passwords.  This means that any printable ASCII password of 9 characters or fewer can be
backed out from the hash.  Longer passwords will be aliased to shorter passwords, and
the reversal function will yield a password that hashes to the same value.

Note that spaces in passwords will be aliased to tilde (~) and any leading carats (^) will
be stripped from the passwords.  This is a consequence of an interaction of the unerlying
mathematics of the hash function with the structure of the ASCII character set, not
a deliberate design decision.

If supporting Unicode, normalize the passwords first, and encode them in UTF-8 before
passing to the hash function.  All passwords 9 characters or fewer and containing only
English printable characters will still be reversible.  Most Eurpean characters will
be aleased to two or three English characters, depending on which Unicode normalization
is used.

The suggested method for generating the 20 base64 digits of the 120-bit obfuscation
key on OS X/Linux/Unix is to read 15 bytes from /dev/urandom and MIME or base64 encode
them as follows:
dd if=/dev/urandom bs=15 count=1 | base64

Examples:
$ dd if=/dev/urandom bs=15 count=1 | base64
1+0 records in
1+0 records out
15 bytes transferred in 0.000056 secs (267722 bytes/sec)
ErQyrC3tXln+5fkV/T1X

$ ./bad_password_hash ErQyrC3tXln+5fkV/T1X A
hash:003C4BEBEC03BF30
password: A

# Either the MIME or URL-safe base64 encodings can be used
$ ./bad_password_hash ErQyrC3tXln-5fkV_T1X A
hash:003C4BEBEC03BF30
password: A

$ ./bad_password_hash ErQyrC3tXln+5fkV/T1X a
hash:0A630D4B99BE8F4B
password: a

$ ./bad_password_hash ErQyrC3tXln+5fkV/T1X abcdefghi
hash:0882D527601A5A3F
password: abcdefghi

$ ./bad_password_hash ErQyrC3tXln+5fkV/T1X abcdefghij
hash:059B4B6D266C3CAD
password: 7/0(@?Rp;

# Different obfuscation keys lead to different aliases for long passwords
$ ./bad_password_hash BBBBBBBBBBBBBBBBBBBB abcdefghij
hash:0611E87A49A1D8C4
password: UJ6<Qt|6/

$ ./bad_password_hash ErQyrC3tXln+5fkV/T1X ^^a
hash:0A630D4B99BE8F4B
password: a

$ ./bad_password_hash ErQyrC3tXln+5fkV/T1X a^a
hash:0074B3DF3A12721B
password: a^a
