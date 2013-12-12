import time
import serial
import smtplib
import urllib2
import urllib
import json
j = urllib2.urlopen("http://pesgif.host56.com/get_ip.php") #open the get_ip.php file on the server, which returns a JSON Object of the IP Address
jraw = j.read()
text = jraw.split('\n', 1)[0]
j_obj = json.loads(text) #parse the JSON Object - IP Address is stored in j_obj['ipad']
#read the url with the HTTP GET Parameter 'ip' = the IP Address of the user; the API returns a JSON Object with the city, region, latitude, longitude, and zip code
response = urllib.urlopen('http://api.ipinfodb.com/v3/ip-city/?key=4316b0d9fe8bd8e7cdda591cb097160d79fbf8781853d318c3833951eb354033&format=json&ip='+j_obj['ipad']).read()
data = json.loads(response) #parse the JSON Object
city = data['cityName'].replace (" ", "_") #replace the spaces in the city name with underscores, do the same for the other values
region = data['regionName'].replace (" ", "_")
lat = data['latitude'].replace (".", "_")
lon = data['longitude'].replace (".", "_")
zipc = data['zipCode']
ser = serial.Serial('COM5', 9600) #Serial monitor to which the Arduino writes
while True:
    message = ser.readline() #Read the serial monitor constantly
    print(message)
    #extract the various values from the string
    crop = message[2]
    sen = message[0]
    read = message[8:].strip()
    perc = message[4:7]
    #pass all the details with readings as GET parameters to upload.php (on the server)
    urlm = "http://pesgif.host56.com/upload.php?id=1&crop="+crop+"&sen="+sen+"&read="+read+"&perc="+perc+"&city="+city+"&region="+region+"&lat="+lat+"&lon="+lon+"&zipc="+zipc
    urlm = str(urlm)
    f = urllib2.urlopen(urlm)
    time.sleep (1); #pause for a second
