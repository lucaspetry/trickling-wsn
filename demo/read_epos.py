#!/usr/bin/env python3

# To get an unencrypted PEM (without passphrase):
# openssl rsa -in certificate.pem -out certificate_unencrypted.pem

import os
import re
import sys
import time
import serial
import argparse
import requests
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='EPOS Serial->IoT Gateway')
parser.add_argument('-d','--dev', help='EPOSMote III device descriptor file', default='/dev/ttyACM0')
parser.add_argument('-t','--timeout', help='Timeout for reading from mote', default=1000)
args = vars(parser.parse_args())

DEV = args['dev']
TIMEOUT = int(args['timeout'])

def init_mote():
    global DEV
    global TIMEOUT

    ok = False
    while not ok:
        try:
            print("Waiting for", DEV, "to appear")
            while not os.path.exists(DEV) or not os.access(DEV, os.W_OK):
                pass
            mote = serial.Serial(DEV, 115200, timeout = TIMEOUT, write_timeout = 10)
            # print(mote.readline())
            ok = True
        except KeyboardInterrupt:
            raise
        except Exception as e:
            print("Exception caught:", e, file=sys.stderr)	
            ok = False
            time.sleep(3)

    print("Mote open", file=sys.stderr)
    ts = bytes(str(int(time.time() * 1000000)), 'ascii')
    try:
        mote.write(ts + b'X')
        print("epoch written", file=sys.stderr)
    except KeyboardInterrupt:
        raise
    except serial.serialutil.SerialTimeoutException:
        pass

    print("init_mote() done", file=sys.stderr)
    return mote

def find_value(var, data):
	re_search = re.search(var+'=((-)?[0-9]+)', str(data))
	value = ''
	if re_search is not None:
		value = re_search.group().split('=')[1]

	return int(value)


if __name__ == "__main__":
	mote = init_mote()

	# session = requests.Session()
	# session.headers = {'Connection': 'close', 'Content-type' : 'application/octet-stream', 'Content-length' : 82}
   
	fig = plt.figure()
	plt.ion()
	plt.axis([1, 40, 0, 2000])
	t = 0

	x_arr = [0]
	last_value_arr = [0]
	trusty_arr = [0]

	while True:
		try:
			lines = 7
			data = b""
			for _ in range(lines):
				data += mote.readline()
		except KeyboardInterrupt:
			raise
		except Exception as e:
			data = b''

		if not len(data):
			mote.close()
			mote = init_mote()
		else:
			predicted = find_value('predicted', data)
			last_value = find_value('last_value', data)
			received = find_value('received', data)
			sync_interval = find_value('sync_interval', data)
			trusty = find_value('trusty', data)
			t += 1

			last_value_arr.append(last_value if last_value > 0 else 0)
			trusty_arr.append(trusty)
			x_arr.append(t)

			plt.draw()
			plt.plot(last_value_arr, color='k')


			value_if_trusty = [i for i, j in zip(last_value_arr, trusty_arr) if j]
			x_if_trusty = [i for i, j in zip(x_arr, trusty_arr) if j]
			plt.scatter(x_if_trusty, value_if_trusty, color='green', s=10)
			value_not_trusty = [i for i, j in zip(last_value_arr, trusty_arr) if not j]
			x_not_trusty = [i for i, j in zip(x_arr, trusty_arr) if not j]
			plt.scatter(x_not_trusty, value_not_trusty, color='red', s=10)
			plt.pause(0.05)

			# print('#############################')

