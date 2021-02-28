# SivakSPO2Heartbeat
Its coppied project from instructables...

Used MAX30102 Sensor and LCD.

See https://www.instructables.com/Pulse-Oximeter-With-Much-Improved-Precision/

GITHUB with original sources: https://github.com/aromring/MAX30102_by_RF



Some change in example is needed for cheap china modules
<pre>
 &nbsp;<font color="#5e6d03">for</font><font color="#000000">(</font><font color="#000000">i</font><font color="#434f54">=</font><font color="#000000">0</font><font color="#000000">;</font><font color="#000000">i</font><font color="#434f54">&lt;</font><font color="#000000">BUFFER_SIZE</font><font color="#000000">;</font><font color="#000000">i</font><font color="#434f54">++</font><font color="#000000">)</font>
 &nbsp;<font color="#000000">{</font>
 &nbsp;&nbsp;&nbsp;<font color="#5e6d03">while</font><font color="#000000">(</font><font color="#d35400">digitalRead</font><font color="#000000">(</font><font color="#000000">oxiInt</font><font color="#000000">)</font><font color="#434f54">==</font><font color="#000000">1</font><font color="#000000">)</font><font color="#000000">;</font> &nbsp;<font color="#434f54">&#47;&#47;wait until the interrupt pin asserts</font>
 &nbsp;&nbsp;&nbsp;<font color="#000000">maxim_max30102_read_fifo</font><font color="#000000">(</font> <font color="#000000">(</font><font color="#000000">aun_ir_buffer</font><font color="#434f54">+</font><font color="#000000">i</font><font color="#000000">)</font><font color="#434f54">,</font><font color="#000000">(</font><font color="#000000">aun_red_buffer</font><font color="#434f54">+</font><font color="#000000">i</font><font color="#000000">)</font><font color="#000000">)</font><font color="#000000">;</font> &nbsp;<font color="#434f54">&#47;&#47;read from MAX30102 FIFO &#47;&#47;swapped values for cheap modules</font>

</pre>

![alt text](https://github.com/sjsivo/Sivak-SPO2-Heartbeat-Sensor/blob/main/disp1.png?raw=true)

![alt text](https://github.com/sjsivo/Sivak-SPO2-Heartbeat-Sensor/blob/main/disp2.png?raw=true)

![alt text](https://github.com/sjsivo/Sivak-SPO2-Heartbeat-Sensor/blob/main/sensorMAX30102.png?raw=true)
