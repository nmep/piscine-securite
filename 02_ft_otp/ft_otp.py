import argparse
import string

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
	
def main():
	parsing()
	# print(1)

if __name__ == "__main__":
	main()
