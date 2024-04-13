from flask import Flask ,Response ,request,redirect,url_for,render_template
from pymongo import MongoClient
import json
from bson.objectid import ObjectId   # we can use the object id as text 


app = Flask(__name__)                # creating an instance of app aplication 

try:  ##connect to database 
    mongo = MongoClient("mongodb+srv://nassim:zamoum@nodetuts.i6qazqe.mongodb.net/sensors_db")
    db = mongo.sensors_db
    mongo.server_info()  # trigger exception if  we can not connect to db 


except : 
    print("error _cznt not connect ot db")

####################################################" updating 
@app.route("/api/sensors/<id>",methods =["PATCH"])
def update_users(id):
    try : 
        t= request.json["temp"]
        print(t)
        dbResponse = db.sensors.update_one({"_id": ObjectId(id)},
        {"$set": {"long" : request.json["long"],"lat" : request.json["lat"],"temp" :request.json["temp"],"pulses" :request.json["pulses"] }}                          
                                         )
        return Response(
            response=json.dumps({"message":" user updated  successfully "}
             ), status=200,
            mimetype="application/json")    

    except Exception as ex :
        print(ex)
        return Response(
            response=json.dumps({"message":"cannot update"}
             ), status=500,
            mimetype="application/json")


####################################################" reading

@app.route("/api/sensors",methods=["GET"])
def get_some_users():
    
    data = list(db.sensors.find())
    for user in data :
        user["_id"] = str(user["_id"])
        #print(user["name"])
  
    return Response(
         
            response=json.dumps(data),
            status=200,
            mimetype="application/json"
            )

#########################  creating
@app.route("/api/sensors",methods=["POST"])
def create_user():
    print("request from esp32 yooha")
    user={"id":request.json["id"],"lat":request.json["lat"],"long":request.json["long"],"temp":request.json["temp"],"pulses":request.json["pulses"] }
    dbResponse = db.sensors.insert_one(user)
    print(dbResponse.inserted_id)
        # for attr in dir(dbResponse):
            
        #     print(attr)
    return Response(
            response=json.dumps({"message":"usercreated" ,"idd": f"{dbResponse.inserted_id}"}
             ), status=200,
                        mimetype="application/json")

            
    
    
#########################
@app.route("/home",methods=["GET"])
def home():
    data = list(db.sensors.find())
    first_document = data[0]
    temp = first_document['temp']
    pulses = first_document['pulses']
    long = first_document['long']
    print("temperature=",temp)
    print("long=",long) 
    t=temp
    l= long 
    p=pulses
    return render_template('main.html',t=t,l=l,p=p)








####################################################
if __name__ == "__main__" :
    app.run(host='0.0.0.0' ,port=5555,debug=True)               # you choose any port you want above  65535
