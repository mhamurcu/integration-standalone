/* Edge Impulse Arduino examples
 * Copyright (c) 2020 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// If your target is limited in memory remove this macro to save 10K RAM
#define EIDSP_QUANTIZE_FILTERBANK 0

/**
 * Define the number of slices per model window. E.g. a model window of 1000 ms
 * with slices per model window set to 4. Results in a slice size of 250 ms.
 * For more info: https://docs.edgeimpulse.com/docs/continuous-audio-sampling
 */
#define EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW 2

#define AT_BAUD_RATE 9600

/* Includes ---------------------------------------------------------------- */
#include <Arduino.h>
#include <PDM.h>
#include <with_regularization_inference.h>
#include <Scheduler.h>
#include <string.h>

/*Include temperature sensor*/
#include <Wire.h>
#include <Adafruit_MLX90614.h>

/*include wifi*/
#include <WiFiEspAT.h>

//include real time utility functions
#include "timeUtil.h"

//Function Declarations
void printWifiStatus();
static void microphone_inference_end(void);
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr);
static bool microphone_inference_record(void);
static bool microphone_inference_start(uint32_t n_samples);
static void pdm_data_ready_inference_callback(void);
void ei_printf(const char *format, ...);
void get_currentTime();
int send_wifi(String postMes, uint8_t whichMeasurement);
void temp_sensor_loop();
void loop3();
void loop2();
void loop_mic();

const uint8_t COUGH_MES = 0;
const uint8_t TEMP_MES = 1;
const uint8_t HEARTRATE_MES = 2;
const uint8_t SPO2_MES = 3;

char *cough_times;
String cough_time_string;
String cough_time_string_array[10];
String tempString;
String notPostedMeas = "[";
int bulkIsNotSent;
unsigned long currentTime = 0;
unsigned long prevTime = 0;

time_t seconds;

int led1 = LEDR;
int led2 = LEDG;
int led3 = LEDB;

int coughnum = 0;
int windowcount = 0;
char *cough_counter_array[500];
char cough_time_to_print[100];
int32_t cough_counter = 0;

/** Audio buffers, pointers and selectors */
typedef struct
{
    signed short *buffers[2];
    unsigned char buf_select;
    unsigned char buf_ready;
    unsigned int buf_count;
    unsigned int n_samples;
} inference_t;

static inference_t inference;
static bool record_ready = false;
static signed short *sampleBuffer;
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
static int print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);

/*Initialize temperature sensor object*/
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

/*Wi-fi variables*/
char server[] = "3.125.238.65";
//const char *server = "192.168.43.178";
WiFiClient client;

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

/**
 * @brief      Arduino setup function
 */
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    while (!Serial)

        pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(9, OUTPUT);

    // Add "loop2" and "loop3" to scheduling.
    // "loop" is always started by default.
    //Scheduler.startLoop(temp_sensor_loop);
    //thread2.start(mbed::callback(loop2));
    Serial.println("Edge Impulse Inferencing Demo");

    // summary of inferencing settings (from model_metadata.h)
    ei_printf("Inferencing settings:\n");
    ei_printf("\tInterval: %.2f ms.\n", (float)EI_CLASSIFIER_INTERVAL_MS);
    ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
    ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) /
                                            sizeof(ei_classifier_inferencing_categories[0]));

    run_classifier_init();
    if (microphone_inference_start(EI_CLASSIFIER_SLICE_SIZE) == false)
    {
        ei_printf("ERR: Failed to setup audio sampling\r\n");
        return;
    }

    /*Temp sensor begin*/
    mlx.begin();

    /* Wi-fi begin*/
    // initialize serial for ESP module
    // initialize ESP module

    Serial1.begin(AT_BAUD_RATE);
    WiFi.init(Serial1);
    /*  String postBody = "{\"UID\":  \"1610054147005\", \"ts\": \"2009-02-22T23:33:02.971Z\", \"temperature\":35.5}";
    */
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println();
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true)
            ;
    }

    // waiting for connection to Wifi network set with the SetupWiFiConnection sketch
    Serial.println("Waiting for connection to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print('.');
    }
    Serial.println();
    Serial.println("Connected to WiFi network.");

    Serial.println("connected to server");

    WiFi.sntp("pool.ntp.org");
    delay(1000);
    char *deneme = "deneme123";
    Serial.println(deneme);
    Serial.println("Waiting for SNTP");

    /*WiFi.getTime() returns length of WiFi.my_tok*/
    Serial.println(WiFi.getTime());
    Serial.println("my token value");
    Serial.println(WiFi.my_tok);
    delay(1000);
    Serial.println();
    // setTime(WiFi.getTime() + (SECS_PER_HOUR * TIME_ZONE));
    /*WiFi.my_tok character arrayi cinsinden asagıdaki gibi dönüyor: 
  *  WiFi.my_tok = "Thu May 13 20:02:49 2021"

  *  
  */
    time_parser();
    Serial.println("Time parsing finished");
    delay(1000);

    /* 
    client.println(("POST /api/v1/symptoms HTTP/1.1"));
    Serial.println("post 1");

   client.println("Connection: keep-alive"); 
       Serial.println("post 2");

   client.println("Content-Type: application/json; charset=utf-8");
       Serial.println("post 3");

   //client.println("Host:epsilon.run-eu-central1.goorm.io");
   client.println("Host:localhost");
       Serial.println("post 4");




    client.println("Content-Length: " + String(postBody.length()));
        Serial.println("post 5");

   client.println();
       Serial.println("post 6");

client.println(postBody);
    Serial.println("post 7");
 


    
  }*/
    // Scheduler.startLoop(loop_mic);

    //Scheduler.startLoopAboveNormal(temp_sensor_loop);
}

/**
 * @brief      Arduino main function. Runs the inferencing loop.
 */
void loop()
{
    //delay(1000);
    while (0)
    {
        int gir = millis();
        if (WiFi.status() == WL_NO_MODULE)
        {
            Serial.println();
            Serial.println("Communication with WiFi module failed!");
            // don't continue
            //gonderemedim biriktir
            //while (true);
        }

        if (WiFi.status() == WL_CONNECTED)
        {

            Serial.println("Connected");
        }
        else
            Serial.println("LOST CONNECTION");
        int cik = millis();
        Serial.println(cik - gir);
    }

    time_t seconds = time(NULL);

    uint8_t whichMeasurement = COUGH_MES;

    Serial.println("ANA LOOP");

    digitalWrite(9, HIGH);
    bool m = microphone_inference_record();
    if (!m)
    {
        ei_printf("ERR: Failed to record audio...\n");
        return;
    }

    signal_t signal;
    signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = {0};

    EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK)
    {
        ei_printf("ERR: Failed to run classifier (%d)\n", r);
        return;
    }

    if (++print_results >= (EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW))
    {
        // print the predictions
        //  ei_printf("Predictions ");
        // ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        //  result.timing.dsp, result.timing.classification, result.timing.anomaly);
        //ei_printf(": \n");
        if (result.classification[0].value > 0.7000)
        {
            cough_time_string = get_my_time();
            //  Serial.println(get_my_time());
            cough_time_string_array[coughnum] = cough_time_string;
            String postMes = "\", \"cough\":true";

            int a = send_wifi(postMes, whichMeasurement);
            digitalWrite(LEDB, LOW);
            coughnum++;
        }
        else
            digitalWrite(LEDB, HIGH);
        windowcount++;
        //ei_printf("    %s: %.5f\n", result.classification[0].label,
        //result.classification[0].value);
        // ei_printf("\n");
        ei_printf("Cough Count: %d", coughnum);
        ei_printf("\n");
        ei_printf("Window Count: %d", windowcount);
        ei_printf("\n");

        /*my implementation*/

#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif

        print_results = 0;
    }
    Serial.println(localtime(&seconds)->tm_sec);

    if (((localtime(&seconds)->tm_sec % 10) < 2))
    {
        temp_sensor_loop();
    }
}
// Task no.2: blink LED with 0.1 second delay.
void loop2()
{
    // if there are incoming bytes available
    // from the server, read them and print them
    while (1)
    {

        Serial.println("trying to send_wifi");
        // send_wifi();
        Serial.println("Waiting for available");
        delay(10000);

        /* while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    delay(5000);*/
        yield();
        // do nothing forevermore
        //while (true);
    }
}

// Task no.3: accept commands from Serial port
// '0' turns off LED
// '1' turns on LED
void loop3()
{
    if (Serial.available())
    {
        char c = Serial.read();
        if (c == '0')
        {
            digitalWrite(led1, LOW);
            Serial.println("Led turned off!");
        }
        if (c == '1')
        {
            digitalWrite(led1, HIGH);
            Serial.println("Led turned on!");
        }
    }

    // IMPORTANT:
    // We must call 'yield' at a regular basis to pass
    // control to other tasks.
    yield();
}

void temp_sensor_loop()
{

    uint8_t whichMeasurement;
    String tempString2;
    Serial.print("Ambient = ");
    Serial.print(mlx.readAmbientTempC() + 4.0);
    Serial.print("*C\tObject = ");
    Serial.print(mlx.readObjectTempC() + 4.0);
    tempString2 = String(mlx.readObjectTempC() + 4.0);
    whichMeasurement = TEMP_MES;
    int x = send_wifi(tempString2, whichMeasurement);

    Serial.println("looptan cıktı");
}

int send_wifi(String postMes, uint8_t whichMeasurement)
{
    //String postBody3= "{\"UID\":  \"1610054147005\", \"ts\": \"2021-05-15T14:06:08.000Z\", \"temperature\":35.5";
    String postBody3 = "{\"UID\":  \"1610054147005\", \"ts\": \"2014-02-22T23:33:02.971Z\", \"temperature\":35.5 , \"cough\":true}]";
    String postBody = "{\"UID\":  \"1\", \"ts\": \"";

    postBody += get_my_time();

    switch (whichMeasurement)
    {
    case COUGH_MES:
        postBody += postMes;
        postBody += "}";
        break;
    case TEMP_MES:
        postBody += "\", \"temperature\":";
        postBody += postMes;
        postBody += "}";
        break;
    case HEARTRATE_MES:
        break;
    case SPO2_MES:
        break;
    default:
        break;
    }
    Serial.println(postBody);
    // postBody += "\", \"temperature\":37.7";
    // postBody += ", \"cough\":true},";

    //String post2= "\", \"cough\":true}";
    // postBody += ", \"cough\":true}";
    // postBody = postBody + post2;

    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println();
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        notPostedMeas = notPostedMeas + postBody + ",";
        return 0; //gonderemedim biriktir
        //while (true);
    }

    // waiting for connection to Wifi network set with the SetupWiFiConnection sketch
    Serial.println("Waiting for connection to WiFi");
    if (WiFi.status() != WL_CONNECTED) //bagli degilse notposted in sonuna ekle
    {
        notPostedMeas = notPostedMeas + postBody +  ",";
        Serial.print('.');
        bulkIsNotSent = 1;
        return 0;
    }
    else if(bulkIsNotSent)
    {

        if (client.connect(server, 80))
        {
            Serial.println("connected to server");

            client.println(("POST /api/v1/bulk-symptoms HTTP/1.1"));
            Serial.println("post 1");

            client.println("Connection: keep-alive");
            Serial.println("post 2");

            client.println("Content-Type: application/json; charset=utf-8");
            Serial.println("post 3");

            client.println("Host:epsilon-backend-nswyg.run-eu-central1.goorm.io");
            // client.println("Host:localhost");
            Serial.println("notpost 4");

            //client.println("Accept: */*");

            client.println("Content-Length: " + String(notPostedMeas.length() + 1));
            Serial.println("notpost 5");

            client.println();
            Serial.println("notpost 6");

            notPostedMeas[notPostedMeas.length() - 1] = ' ';
            notPostedMeas += "]";
            Serial.println(notPostedMeas);

            client.println(notPostedMeas);
            Serial.println("notpost 7");
            notPostedMeas = '[';
        }

        bulkIsNotSent = 0;
    }

    Serial.println();
    Serial.println("Connected to WiFi network.");

    Serial.println("Starting connection to server...");
    if (client.connect(server, 80))
    {
        Serial.println("connected to server");

        client.println(("POST /api/v1/symptoms HTTP/1.1"));
        Serial.println("post 1");

        client.println("Connection: keep-alive");
        Serial.println("post 2");

        client.println("Content-Type: application/json; charset=utf-8");
        Serial.println("post 3");

        client.println("Host:epsilon-backend-nswyg.run-eu-central1.goorm.io");
        // client.println("Host:localhost");
        Serial.println("post 4");

        //client.println("Accept: */*");

        client.println("Content-Length: " + String(postBody.length()));
        Serial.println("post 5");

        client.println();
        Serial.println("post 6");

        client.println(postBody);
        Serial.println("post 7");
    }
    return 1;
}

void get_currentTime()
{
}

/**
 * @brief      Printf function uses vsnprintf and output using Arduino Serial
 *
 * @param[in]  format     Variable argument list
 */
void ei_printf(const char *format, ...)
{
    static char print_buf[1024] = {0};

    va_list args;
    va_start(args, format);
    int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);

    if (r > 0)
    {
        Serial.write(print_buf);
    }
}

/**
 * @brief      PDM buffer full callback
 *             Get data and call audio thread callback
 */
static void pdm_data_ready_inference_callback(void)
{
    int bytesAvailable = PDM.available();

    // read into the sample buffer
    int bytesRead = PDM.read((char *)&sampleBuffer[0], bytesAvailable);

    if (record_ready == true)
    {
        for (int i = 0; i < bytesRead >> 1; i++)
        {
            inference.buffers[inference.buf_select][inference.buf_count++] = sampleBuffer[i];

            if (inference.buf_count >= inference.n_samples)
            {
                inference.buf_select ^= 1;
                inference.buf_count = 0;
                inference.buf_ready = 1;
            }
        }
    }
}

/**
 * @brief      Init inferencing struct and setup/start PDM
 *
 * @param[in]  n_samples  The n samples
 *
 * @return     { description_of_the_return_value }
 */
static bool microphone_inference_start(uint32_t n_samples)
{
    inference.buffers[0] = (signed short *)malloc(n_samples * sizeof(signed short));

    if (inference.buffers[0] == NULL)
    {
        return false;
    }

    inference.buffers[1] = (signed short *)malloc(n_samples * sizeof(signed short));

    if (inference.buffers[0] == NULL)
    {
        free(inference.buffers[0]);
        return false;
    }

    sampleBuffer = (signed short *)malloc((n_samples >> 1) * sizeof(signed short));

    if (sampleBuffer == NULL)
    {
        free(inference.buffers[0]);
        free(inference.buffers[1]);
        return false;
    }

    inference.buf_select = 0;
    inference.buf_count = 0;
    inference.n_samples = n_samples;
    inference.buf_ready = 0;

    // configure the data receive callback
    PDM.onReceive(&pdm_data_ready_inference_callback);

    // optionally set the gain, defaults to 20
    PDM.setGain(90);

    PDM.setBufferSize((n_samples >> 1) * sizeof(int16_t));

    // initialize PDM with:
    // - one channel (mono mode)
    // - a 16 kHz sample rate
    if (!PDM.begin(1, EI_CLASSIFIER_FREQUENCY))
    {
        ei_printf("Failed to start PDM!");
    }

    record_ready = true;

    return true;
}

/**
 * @brief      Wait on new data
 *
 * @return     True when finished
 */
static bool microphone_inference_record(void)
{
    bool ret = true;

    if (inference.buf_ready == 1)
    {
        ei_printf(
            "Error sample buffer overrun. Decrease the number of slices per model window "
            "(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)\n");
        ret = false;
    }

    while (inference.buf_ready == 0)
    {
        delay(1);
    }

    inference.buf_ready = 0;

    return ret;
}

/**
 * Get raw audio signal data
 */
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    numpy::int16_to_float(&inference.buffers[inference.buf_select ^ 1][offset], out_ptr, length);

    return 0;
}

/**
 * @brief      Stop PDM and release buffers
 */
static void microphone_inference_end(void)
{
    PDM.end();
    free(inference.buffers[0]);
    free(inference.buffers[1]);
    free(sampleBuffer);
}

void printWifiStatus()
{
    // print the SSID of the network you're attached to
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength
    long rssi = WiFi.RSSI();
    Serial.print("Signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_MICROPHONE
#error "Invalid model for current sensor."
#endif
