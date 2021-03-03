# does quitting work?
echo -en "q\n" | ./mer_fsm

# does encoding work?
echo -en "1\n2\n3\n4\n5\n6\n7\n" | ./mer_fsm

# does reset encoding work?
echo -en "1\n2\n3\n4\ns\n.\n5\n6\n7\n5\n6\n7\n8\n" | ./mer_fsm


