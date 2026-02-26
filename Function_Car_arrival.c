/*


FUNCTION car_arrival (max cars arriving)
    RAND(0-max cars arriving) = i
    
    FOR (i > 0) DO 
        RAND(0-100) = x

        FOR (x =< prob. of arrival) DO
            new car
            
            IF (car cell free) THEN
                car parks
            ELSE
                car gets into queue
            END IF

        END FOR

        i-1
    END FOR

END FUNCTION











*/