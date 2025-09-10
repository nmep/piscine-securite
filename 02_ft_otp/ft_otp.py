import argparse
from hashlib import sha1
import hmac, binascii

parser = argparse.ArgumentParser()
parser.add_argument("-g", help="save the hexadecimal key of 64bit length give in output as G, it can be a string or writed in a file")
parser.add_argument("-k", help="generates a new temporary password based on the key given as K (it must be the ft_otp.key created by -g) and prints it on the standard output")
args = parser.parse_args()

def parsing() -> None:
	if args.g:
		buff = None
		try:
			with open(args.g, "r") as f:
				buff = f.read(70).replace("\n", "")
				if len(buff) != 64:
					print(f"./ft_otp.py error: key length must be 64 and is {len(buff)}")
					exit(1)
		except FileNotFoundError:
			pass

		if len(args.g) == 64 and buff is None:
			buff = args.g

		if buff is None:
			print(f"./ft_otp.py error: key not found in {args.g}")
			exit(1)

		try:
			with open("ft_otp.key", "w") as f:
				f.write(buff)
			print("Key was successfully saved in ft_otp.key.")
		except Exception as e:
			print(f"./ft_otp.py error: {e}")

def counter() -> int:
	file = ".counter.txt"
	val = 0
	try:
		with open(file, "r") as f:
			val = int(f.read())
	except Exception:
		pass
	val += 1
	with open(file, "w") as f:
		f.write(str(val))
	return val

def sha1_algo(key: bytes, count: int) -> bytes:
	counter_big = count.to_bytes(8, 'big')
	return hmac.new(key, counter_big, sha1).digest()

def hotp_algo(key: bytes, count) -> None:
	print(f"count = {count}")
	hs = sha1_algo(key, count)
	offset = hs[19] & 0xf
	bin_code = (hs[offset] & 0x7f) << 24 | (hs[offset+1] & 0xff) << 16 | \
			   (hs[offset+2] & 0xff) << 8 | (hs[offset+3] & 0xff)
	print(f"hotp code = {bin_code % 10**6}")

def get_key_from_file(key_file: str) -> bytes:
	try:
		with open(key_file, "r") as f:
			key = f.read().strip()
			print(f"key loaded = {key} and len = {len(key)}")
	except Exception as e:
		print(e)
		exit(1)
	return binascii.unhexlify(key)

def main() -> None:
	parsing()
	if args.k:
		key = get_key_from_file(args.k)
		count = counter()
		hotp_algo(key, count)

if __name__ == "__main__":
	main()
