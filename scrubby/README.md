# Description
Loops incoming audio at a user defined interval.  
Use the simple controls to record loops, play and pause them, and record over them.   
Loops can be very long, or very short.  
  
# Controls
| Control | Description | Comment |
| --- | --- | --- |
| Button 1 | Play / Pause| |
| Button 2 | Record | Press to record/SOS. Hold to reset |
| Leds | Modes | Red: record enable. Green: Playback |
| Knob 1 | Live In / Loop | Left is in, right is loop |

# Diagram
<img src="https://raw.githubusercontent.com/electro-smith/DaisyExamples/master/pod/Looper/resources/Looper.png" alt="Button_schem.png" style="width: 100%;"/>

# Code Snippet  
```cpp    
void NextSamples(float &output, float* in, size_t i)  
{  
    if (rec)  
    {  
        WriteBuffer(in, i);  
    }  
    
    output = buf[pos];
    
    ......

    if(play)
    {
        pos++;
        pos %= mod;
    }

    if (!rec)
    {
        output = output * drywet + in[i] * (1 -drywet);
    }
}  

```
