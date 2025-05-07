from flask import Flask, request, jsonify
from flask_cors import CORS  
import requests
import time

app = Flask(__name__)
CORS(app)  

latest_data = {
    "pH_Value": None,
    "Turbidity_Value": None
}

britaTotal = []
britaLimit = 46
totalReadings = 0
@app.route('/')
def receive_data():
    phValue = request.args.get('phValue')
    ntu = request.args.get('ntu')
    global totalReadings
    if phValue and ntu:
        totalReadings += 1
        latest_data["pH_Value"] = float(phValue)
        latest_data["Turbidity_Value"] = float(ntu)
        britaTotal.append(latest_data)  
        print(f" Current water values: pH_level={phValue}, Turbidity_Value={ntu}")
        waterQuality()
        return f"Data received: pH Level={phValue}, Turbidity={ntu}"
    return "No data received."

@app.route('/data')
def get_data():
    return jsonify({
        "pH": latest_data.get("pH_Value"),
        "turbidity": latest_data.get("Turbidity_Value")

    })
@app.route('/britaTotal/count')
def get_brita_count():
    return jsonify({"Brita Level":britaLimit -  len(britaTotal)})


def sendMessage(message):
    bot_token = '7538944499:AAGlDmGsse0ssLyEOZjdpQVBAx2Vjmp_jOM'  
    chat_id = '7865532985'      
    url = f"https://api.telegram.org/bot{bot_token}/sendMessage"

    payload = {
        'chat_id': chat_id,
        'text': message
    }

    try:
        response = requests.post(url, data=payload) 
        if response.status_code == 200:
            print(" Telegram message sent!")
        else:
            print(f" Failed to send message. Status: {response.status_code}")
            print(response.text)
    except Exception as e:
        print(f" Error: {e}")

def waterQuality():
    global totalReadings

    if (latest_data['Turbidity_Value'] is not None and
        latest_data['pH_Value'] is not None and
        totalReadings >= 10):

        totalReadings = 0
        remaining = britaLimit - len(britaTotal)
        ph = latest_data['pH_Value']
        turb = latest_data['Turbidity_Value']

        if turb < 5.0 and 6.5 <= ph <= 8.5 and remaining > 10:
            sendMessage(f"Water is fantastic. You have approximately {remaining} refills left until the filter needs to be changed.")

        elif 5.0 <= turb < 10.0 and 6.5 <= ph <= 8.5 and remaining > 3:
            sendMessage(f"Water is decent. You have approximately {remaining} refills left until the filter needs to be changed.")
            time.sleep(2)
            sendMessage("It is recommended to order a new Brita filter soon.")

        else:
            sendMessage("Water quality is poor. Please change the filter immediately.")

