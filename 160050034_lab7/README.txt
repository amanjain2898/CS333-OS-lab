
0. (i) acquire() and release() for obtaining and releasing lock respectively
   (ii) The holding function checks whether the current cpu holds the lock
   (iii) Add another acquire statement "acquire(&ptable.lock)" just below the acquire lock statement in allocproc() function in proc.c. Hence two consecutive calls of the acquire function will result in panic in the second call since the holding condition returns true.
   (iv) xchg : atomic statement that combines the two statements of checking lock status and changing lock to 1 after 
   			   acquiring the lock
   		pushcli: It calls the cli() function which disables interrupts and also keeps count of the number of times the
   		  		 pushcli() has been called to track the number of processes requesting the lock
   		popcli : it calls sti() function which reenables interrupts only when the cli count becomes 0 and decrements the 
   		         cli count by 1 in each call

    (v) The sti() call enables the interrupts after the acquire statement which disrupts the lock functioning as the interrupts need to be disabled after the acquiring of the lock as then deadlock state can arise when the code segment between the acquire and release statements is executed and another interrupt occurs on the cpu.

 


