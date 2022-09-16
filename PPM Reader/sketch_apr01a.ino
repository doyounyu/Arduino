#define SAMPLING_TIME    (double) (2.0) //[ms] 단위
#define  MAX_CH          (int)    (8)  // 채널 개수
#define  SYNC_TIME       (int)    (4000) // Synchro Blank Time 길이. 최소 Blank time 을 기준으로 할 것


//t_now: FALLING 발생 시 시간  t_gap: FALLING 간 시간차  t_prev: 이전 FALLING 발생 시간
unsigned long int t_now = 0, t_gap = 0, t_prev = 0; 




/*==================채널 변수=====================*/

double CH[MAX_CH] = {0.0,};
double CH_filtered[MAX_CH] = {0.0,};

 
/*==================Filter 용 변수=====================*/
double xn1 = 0.0;
double yn1 = 0.0;



//8번째 채널 -> chan_num = 7
int chan_num = 0;

/*==================Real-Time 용 변수=====================*/
double loop_start_time = 0.0;
double loop_end_time = 0.0;
double loop_time[2000] = {0.0,};
int loop_count = 0;
void setup() 
{

  Serial.begin(9600);
 pinMode(0, INPUT_PULLUP); //???
  attachInterrupt(digitalPinToInterrupt(0), read_me, FALLING);


}

void loop() 
{
loop_start_time = micros();
  //각 시리얼 값 출력

  Serial.print(CH[0]); Serial.print("\t");
  //Serial.print(CH[1]); Serial.print("\t");
 // Serial.print(CH[2]); Serial.print("\t");
 // Serial.print(CH[3]); Serial.print("\t");
 // Serial.print(CH[4]); Serial.print("\t");
//Serial.print(CH[5]); Serial.print("\t");
 // Serial.print(CH[6]); Serial.print("\t");
 // Serial.print(CH[7]); Serial.print("\n");

//54.045⋅yi = (1⋅xi + 1⋅xi-1) + 52.045⋅yi-1


double yn = (1/54.045)*yn1 + (1/54.045)*CH[0] + 0.963 * xn1;
  Serial.print(yn); Serial.print("\n");
xn1 = CH[0];
yn1 = yn;

  while (1)
  {
    loop_end_time = micros();
    if (loop_end_time >= (loop_start_time + (SAMPLING_TIME)/1000))break;
  }


}




void read_me()  
{
  t_now = micros(); //falling 시에 시간값 저장
// Serial.print(chan_num); Serial.print("\t"); Serial.println(t_gap); //Serial.print("\t 현재시간: "); Serial.println(t_now);  //값 보이기
  
  t_gap = t_now - t_prev;  //두 피크간 값 측정
  t_prev = t_now; // 이전 시간 업데이트
//Serial.println(t_gap);
  //t_gap이 SYNC_TIME보다 작을 때만 삽입
  if(t_gap < SYNC_TIME)
  {
    CH[chan_num] = t_gap;
    chan_num++;
    
  }

  
  //8채널이 꽉 차거나, 싱크 타임 이상으로 t_gap이 발생하면 채널 초기화.
  if(chan_num > MAX_CH || t_gap > SYNC_TIME)
  {
    chan_num = 0;
  }


}
