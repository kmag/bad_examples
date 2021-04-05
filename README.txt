A collection of pieces of code used to demonstrate why certain things are bad ideas.

# bad_merkle_tree
A demonstration of what's wrong with a naive Merkle Tree implementation, such as used by BitTorrent and IPFS.
As long as you always sign the file length and the tree root, and always check both, there isn't a security problem with a naive Merkle tree.  However, you're left with greater attack surface, and you're left with an N+M-bit identifier only providing N bits of second-primage resistance.  You're better off using Joan Daemen's (of AES and SHA-3 fame) Sakura Tree construction, which is provably as strong as the underlying hash function.  (One could also go with the old Tiger Tree construction using different 1-byte prefixes for internal and leaf nodes when hashing, but you're better off just going with Sakura Trees.)

# bad_password_hash
This was written in response to a question about the fool's errand of creating a secure and  invertible password hash.
Any hash is trivially invertible to a password of up to 9 characters long.  If your password is longer, inversion will result in a password that isn't yours, but results in the same hash given the same salt.  Really, use Argon2 or scrypt and push back hard if someone tells you invertibility is a business requirement.
