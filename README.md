<h1>Portable Environment Sensing Gadget for Indian Farmers</h1>
This project was originally started as an entry for the <a href = "https://www.googlesciencefair.com/" target = "_blank">Google Science Fair, 2013</a>, and his since been developed majorly.
The complete report that was submitted can be found <a href = "http://goo.gl/sXhZJ" target = "_blank">here</a>.

<h2>What is it?</h2>
In short, its a device that is based on the Arduino microcontroller that brings together an array of environment sensors.
<ul>
	<li>Soil Moisture Sensor</li>
	<li>Temperature Sensor</li>
	<li>Light Sensor (Photoresistor)</li>
	<li>Humidity Sensor</li>
</ul>
In addition, one can select any one of the five popular Indian crops (that are currently supported) at a time.
<ul>
	<li>Rice</li>
	<li>Wheat</li>
	<li>Millets</li>
	<li>Pulses</li>
	<li>Sugarcane</li>
</ul>

<h2>What's new?</h2>
The difference between normal agricultural devices and this, is that this aims to <b>eliminate all numerical output</b>. Sensed values are compared to the ideal values of the selected crop, and the relative reading is converted to <b>visual output</b> through LEDs.

<h2>How does it help?</h2>
Agriculture is a sector in which a major part of India's population is involved in - given that India is the world's second most populous country, and growing fast, this involvement is huge. However, it hasn't lived up to its full potential in the years gone by. 
A lack of <b>modern agricultural tools</b> and <b>scientific knowledge</b> has led to a lower-than-expected produce in vast areas in the country. In many places crops are grown under <b>non-optimal conditions</b>, as farmers rely on their instincts, and even superstitions at times. 
Often, certain crops are grown in completely <b>incompatible conditions</b>.<br><br>
This device is easy-to-use, and the output is <b>intuitive</b> - farmers can use the device to check the closeness of the environmental conditions to the ideal values for their crop, or even check which crops are the most compatible with their region.

<h2>Internet Connectivity</h2>
For demonstration of internet connectivity, which would ultimately be achieved through an Ethernet shield, a Python script has been used. This system will only work when the Arduino is connected to an Internet-connected PC via USB, and the Python script is running simultaneously.
<br><br>When a button on the device is long-pressed, it prints the data to the <b>Serial Monitor</b> of the Arduino. The Python script constantly listens for data at the Serial Port, and when it does, decodes it to obtain the data of the Arduino. Using an <a href = "http://ipinfodb.com/">API on the web</a>, 
details about the device's <b>geographical location</b> are obtained. The script then passes these variables to a PHP script on the <a href = "http://pesgif.host56.com">web server</a>, which inserts the data as a row in a <b>MySQL table</b>. Thus, readings can be recorded and stored on a remote server - 
these can be used for research purposes by agricultural agencies.
