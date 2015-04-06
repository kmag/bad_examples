#!/usr/bin/env python3

from hashlib import sha256
from base64 import urlsafe_b64encode 
from base64 import urlsafe_b64decode as b64decode

def b64encode(b):
  return urlsafe_b64encode(b).decode('utf8')

file0 = b'A'
file1 = b'B'
file2 = b'C'
file3 = b'D'
file4 = b64decode(b'ayPA1fNdGxH5toPwsKYXNV3rESd9ka4JHTmcZVuHlA0_OdXDSOW3nQboQsEU5sxXFYO79E5LDr_aGgHsBXRdQw==')

entity0 = [ file0, file1, [ file2, file3 ] ]
entity1 = [ file0, file1, file4 ]

def wrong_way_to_do_it(entity):
  '''BEP 30 / original THEX proposal wrong way to implement a Merkle tree'''
  result = sha256()
  if type(entity) == bytes:
    # Hashing a file
    result.update(entity) 
  elif type(entity) == list:
    # Hashing a directory 
    for e in entity:
      result.update(wrong_way_to_do_it(e))
  else:
    raise TypeError('Unsupported type')

  return result.digest()

def less_wrong(entity):
  '''Almost Joan Daemen's Sakura encoding, but really use Sakura instead.'''
  result = sha256()

  if type(entity) == bytes:
    # Hashing a file
    result.update(entity + b'\x00')
  elif type(entity) == list:
    # Hashing a directory
    for e in entity:
      result.update(less_wrong(e) + b'\x01')
  else:
    raise TypeError('Unsupported type')

  return result.digest()

if __name__ == '__main__':
  wrong0 = wrong_way_to_do_it(entity0)
  wrong1 = wrong_way_to_do_it(entity1)
  print('%s == hash(%s)' % (b64encode(wrong0), str(entity0)))
  print('%s == hash(%s)' % (b64encode(wrong1), str(entity1)))
  if wrong0 != wrong1:
    print('\nCollision avoided.')
  else:
    print('\nDrat! Hash collision ate all my data!')
  if less_wrong(entity0) != less_wrong(entity1):
    print("Use Joan Daemen's Sakura construction instead!") 
