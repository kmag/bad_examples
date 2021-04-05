A collection of pieces of code used to demonstrate why certain things are bad ideas.

# bad_merkle_tree
A demonstration of what's wrong with a naive Merkle Tree implementation, such as used by BitTorrent and IPFS.  As long as you always sign the file length and the tree root, and always check both, there isn't a security problem with a naive Merkle tree.  However, you're left with greater attack surface, and you're left with an N+M-bit identifier only providing N bits of second-primage resistance.  You're better off using Joan Daemen's (of AES and SHA-3 fame) Sakura Tree construction, which is provably as strong as the underlying hash function.  (One could also go with the old Tiger Tree construction using different 1-byte prefixes for internal and leaf nodes when hashing, but you're better off just going with Sakura Trees.)

# bad_password_hash
An example of how not to perform password hashing.  This hash is reversible for passwords up to 9 English characters in length.  Its computation is much too fast for serious use.  It was written in response to a question about the fool's errand of creating a secure and  invertible password hash.
