from flask import *  
import sqlite3  
import string
import random
import logging
import traceback
import time


app = Flask(__name__)  

@app.route("/save",methods = ["GET"])  
def saveDetails():
	try:  
		print(request.args)
		id = request.args.get('id')
		temp = request.args.get('temp')
		humid = request.args.get('humid')
		app.logger.info(id)
		app.logger.info(temp)
		with sqlite3.connect("employee.db") as con:  
			cur = con.cursor()  
			app.logger.info('''insert into Temperature (name, temp, humidity, time) values
				("{id}",{temp}, {humid}, {time})
				'''.format(id = id, temp = temp, humid=humid, time = round(time.time() * 1000)))
			cur.execute('''insert into Temperature (name, temp, humidity, time) values
				("{id}",{temp}, {humid}, {time})
				'''.format(id = id, temp = temp, humid=humid, time = round(time.time() * 1000)))  
			con.commit()
	except:  
		traceback.print_exc()
		con.rollback()  
		msg = "We can not add the employee to the list"  
	finally:  
		return "done"  
		con.close()  

@app.route("/temp",methods = ["GET"])  
def getTemp():
	rows = ""
	data = []
	try:  
		id = request.args.get('id')
		limit = request.args.get('limit')
		app.logger.info(id)
		with sqlite3.connect("employee.db") as con:  
			cur = con.cursor()  
			cur.execute("select id, temp, humidity, time from Temperature where name = {id} order by time desc limit {limit}".format(id = id, limit=limit))
			rows = cur.fetchall()
			
			for r in rows:
				if(r[0] is None or r[1] is None or r[2] is None or r[3] is None):
					continue
				innerData = {"id":r[0], "temperature": r[1], "humidity": r[2], "time": r[3]}
				data.append(innerData)
			
	except:
		traceback.print_exc()
		pass
	finally:  
		return jsonify(data)
		con.close() 

if __name__ == "__main__":  
    app.run(debug = True)  