import datetime

while True :
    
    if datetime.datetime.now() >= datetime.datetime(2022,10,17,  13,59  ,00,000) :
        print('work')
        break
    else :
        print(str(datetime.datetime.now()))
