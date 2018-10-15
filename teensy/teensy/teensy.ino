#include <EEPROM.h>

int pose = 0;
int pid[3][3];
int previousError[3];
int integral[3];

int cycleFrequency;
int cycleTime;
unsigned long timer = 0;


void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(100);

    int eeaddress = 0;
    for (int i=0;i<=2;i++)
    {
        for (int j=0;j<=2;j++)
        {
            EEPROM.get(eeaddress, pid[i][j]);
            eeaddress += 10;
        }
    }

    EEPROM.get(100,cycleFrequency);
    cycleTime = 1000000/cycleFrequency; //in microsec

}

void updatePIDvalue(int joint, char letter, int value)
{
    int j;
    switch(letter)
    {
        case 'p':
            j=0;
            break;
        case 'i':
            j=1;
            break;
        case 'd':
            j=2;
            break;
        default:
            return;
    }
    pid[joint][j] = value;
    EEPROM.put(joint*30+j*10, value);
}

void printPIDvalues()
{
    Serial.println("Current PID values:");
    for(int i=0;i<=2;i++)
    {        
        Serial.print("Joint ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(pid[i][0]);
        Serial.print(", ");
        Serial.print(pid[i][1]);
        Serial.print(", ");
        Serial.print(pid[i][2]);
        Serial.print("\n\r");
    }
    Serial.print("Frequency: ");
    Serial.println(cycleFrequency);
}

int pidCalculate(int joint, int desired, int actual)
{
    int error = desired - actual;
    integral[joint] = integral[joint] + (error * cycleTime);
    int derivative = (error - previousError[joint]) / cycleTime;
    previousError[joint] = error;
    
    return ((pid[joint][0]*error) + (pid[joint][1]*integral[joint]) + (pid[joint][2]*derivative));
}

void loop() {
    if (Serial.available() > 0)
    {
        switch (Serial.read())
        {
            case 'a':
                pose = Serial.parseInt();
                break;
            case 'j':
                {
                int joint = Serial.parseInt();
                char pid = Serial.read();
                int value = Serial.parseInt();
                updatePIDvalue(joint, pid, value);
                break;
                }
            case 'r':
                printPIDvalues();
                break;
            case 'f':
                cycleFrequency = Serial.parseInt();
                EEPROM.put(100,cycleFrequency);
            default:
                Serial.find(',');
                Serial.println("ERROR: Unknown command");
                break;
        }
    }

    if(micros()-timer > cycleTime)
    {
        float rateError = micros()-timer-cycleTime;
        if(rateError > cycleTime * 0.1)
        {
            Serial.print("WARNING: Missed target rate by ");
            Serial.print(1000000/rateError);
            Serial.println(" Hz");
        }
        timer = micros();
        pidCalculate(1,1,1);
    }
}