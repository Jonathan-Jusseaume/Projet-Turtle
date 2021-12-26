
debug=0
while test $debug -eq 0; do
  debug=$(bash update.sh && (grep 'Timeout! Aucune commande reçue dans les temps.' < turtlepicross/html/results.json | wc -l))
  grep 'Timeout' < turtlepicross/html/results.json
  echo $debug
done