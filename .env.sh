  #!/bin/sh

export MAX_PROMOTOR=10
export MAX_USERS=20
export MAX_ACTIVE_AUCTIONS=30
export FPROMOTERS="file_promoters.txt"
export FUSERS="file_users.txt"
export FITEMS="file_items.txt"

echo "############################################"
echo "###     SETTING ENVIRONMENT VARIABLES    ###"
echo "############################################"
echo "###  MAX_PROMOTOR        ==> $MAX_PROMOTOR          ###"
echo "###  MAX_USERS           ==> $MAX_USERS          ###"
echo "###  MAX_ACTIVE_AUCTIONS ==> $MAX_ACTIVE_AUCTIONS          ###"
echo "############################################"
echo "###  FUSERS      ==> $FUSERS      ###"
echo "###  FPROMOTERS  ==> $FPROMOTERS  ###"
echo "###  FITEMS      ==> $FITEMS      ###"
echo "############################################"