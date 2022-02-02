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
		id = request.args.get('id')
		temp = request.args.get('temp')
		app.logger.info(id)
		app.logger.info(temp)
		with sqlite3.connect("employee.db") as con:  
			cur = con.cursor()  
			app.logger.info('''insert into Temperature (name, temp, time) values
				("{id}",{temp}, {time})
				'''.format(id = id, temp = temp, time = round(time.time() * 1000)))
			cur.execute('''insert into Temperature (name, temp, time) values
				("{id}",{temp}, {time})
				'''.format(id = id, temp = temp, time = round(time.time() * 1000)))  
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
	try:  
		id = request.args.get('id')
		app.logger.info(id)
		with sqlite3.connect("employee.db") as con:  
			cur = con.cursor()  
			cur.execute("select temp from Temperature where id = {id}".format(id = id))
			rows = cur.fetchall()
			app.logger.info(rows)
	except:
		traceback.print_exc()
		pass
	finally: 
		return jsonify(rows[0][0]) 
		con.close() 

if __name__ == "__main__":  
    app.run(debug = True)  