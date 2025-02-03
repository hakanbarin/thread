f(pthread_mutex_trylock(&stoveMutex[i]) == 0){
                    
                int fuelNeeded = (rand() % 20);
                
                    if(stoveFuel[i] - fuelNeeded < 0){
                        printf("fuel is empty");
                    }
                    else{
                        printf("stove...%d\n",stoveFuel[i]);
                        usleep(500000);
                        stoveFuel[i] -= fuelNeeded;
                    }
                pthread_mutex_unlock(&stoveMutex[i]);
                break;
            }
            else
            {
                if (i == 3){
                    usleep(300000);
                    i = 0;
                }
            }