**2022-10-11**

void ESPMachineUI::initEncoder1(long currentValue, long minValue, long maxValue, long steps, boolean invert, bool circleValues)
{
    encodersReader.setScale1(minValue, maxValue, steps, invert, circleValues);
}

   void attachEncoder(ESPMachine dreamachine); // should probably extract interface

    void updateDisplay(string label, string value);

    void initEncoder1(long currentValue, long minValue, long maxValue, long steps, boolean invert, bool circleValues);


    ---

    