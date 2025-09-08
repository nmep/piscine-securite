import argparse
import string
from hashlib import sha1
import os
import hmac
import base64

parser = argparse.ArgumentParser()
parser.add_argument("-g", help="save the hexadecimal key of 64bit length give in output as G, it can be a string or writed in a file")
parser.add_argument("-k", help="generates a new temporary password based on the key given as K (it must be the ft_otp.key created by -g) and prints it on the standard output")
args = parser.parse_args()

def parsing():
	key = 0
	hex_key_file = None
	buff = None

	if args.g:
		# is the key in a file ?
		try:
			hex_key_file = open(args.g, mode="r+")
			buff = hex_key_file.read(65)
			if (len(buff) != 64):
				print(f"./ft_otp.py error: key lenght must be 64 and is {len(buff)}")
				exit(1)
		except FileNotFoundError:
				pass
		except Exception as e:
			print(f"./ft_otp.py error: {e}")

		# is the key is in a string ?
		if (len(args.g) == 64 and buff == None):
			if (all(c in string.hexdigits for c in args.g)):
				try:
					hex_key_file = open("ft_otp.key", mode="w+")
				except Exception as e:
					print(f"./ft_otp.py error: {e}")
				buff = args.g

		print(f"buff = {buff}")
		if (buff == None):
			print(f"./ft_otp.py error: key not found in {args.g}")
			exit(1)
		try:
			hex_key_file = open("ft_otp.key", mode="w")
			hex_key_file.write(buff)
			print("Key was successfully saved in ft_otp.key.")
		except Exception as e:
			print(f"./ft_otp.py error: {e}")

	if (args.k != "ft_otp.key"):
		print("./ft_otp.py error: [-k] only ft_otp.key is accepted as input file")
		exit(1)

def counter():
	file = ".counter.txt"

	if os.path.exists(file):
		try:
			with open(file, 'r') as f:
				val = int(f.read())
		except Exception as e:
			print(e)
	else:
		val = 0
	val += 1

	with open(file, 'w') as f:
		f.write(str(val))
	return val

def sha1_algo(count):
	key = bytes("toto", 'UTF-8')
	msg = bytes(str(count), 'UTF-8')

	digester = hmac.new(key, msg, sha1)

	signature1 = digester.digest()

	signature2 = base64.urlsafe_b64encode(signature1).decode('UTF-8')
	return str(signature2)

def hotp_algo(count):
	print(f"counter = {count}")
	hs = sha1_algo(count)
	hs = hs[:20]
	print(f"res hash sh1 = {hs}")
	offset = ord(hs[19]) & 0xf
	
	bin_code = (hs[offset] & 0x7f) << 24 \
		| (hs[offset+1] & 0xff) << 16 \
		| (hs[offset+2] & 0xff) << 8 \
		| (hs[offset+3] & 0xff)
	# je dois mettre qqc en big endian 
	# je dois calculer une valeur avec deux truc que je connais pas
	#  puis faire le calcul voir rfc
	print(f"offset = {offset}")	
	

def main():
	parsing()
	# faire l'algo que si k est demande

	hotp_algo(counter())
	print(1)


if __name__ == "__main__":
	main()


# sur hs je prend le offsetBits qui est les 4 bit de poids faible de hs[19] (sa valeur est donc entre 0 et 15 inclus)
# je peux aussi prendre 
