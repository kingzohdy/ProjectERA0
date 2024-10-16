ipcs |awk '{if($6==0) printf "ipcrm shm %d\n",$2}' |sh ; ipcs
