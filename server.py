from flask import Flask
from flask import request
from collections import deque

app = Flask(__name__)
queue = deque()

@app.route("/". , methods = ['POST'])
def hello():
	queue.append((request.args.get("X"), request.args.get("Y")))
	return "X: " + str(request.args.get("X")) + "\nY: " + str(request.args.get("Y"))

@app.route("/request/", methods = ['GET']):
def _request():
	if(queue):
		value = queue.popleft()
		return str(value[0]) + " " + str(value[1])
	return "NULL"

@app.route("reset/")
def _reset():
	if(queue):
		queue.clear()
		return "Queue Emptied"
	return "NULL"