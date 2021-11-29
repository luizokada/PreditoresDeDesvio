# PreditordeDevio
9 variações do previsor de desvio de 2 niveis, a iplementação feita para ser utilizada junto com a ferramenta PIN
# 1 set os caminhos
#exemplo do caminho_PIN  /home/luizokada/Desktop/pin nesse caso a pasta do pin estava em
#home/luizokada/Desktop e o executável do pin em /home/luizokada/Desktop/pin 
export PIN_ROOT=caminho_PIN/pin
export PATH=$PIN_ROOT:$PATH
echo -e "\n# environment variables for branch prediction lab" >> ~/.profile
echo "export PIN_ROOT=$PIN_ROOT"   >> ~/.profile
echo 'export PATH=$PIN_ROOT:$PATH' >> ~/.profile

# 2. Para contruir os arquivos .o dos previsores basta digitar
make 
# 3. Test run.
#    pin -t obj-intel64/<bp>.so -- <program>
#      <bp>      - the branch predictors (e.g. bp_btfn.so)
#      <program> - any executable file
#    Results are in stats.out.
pin -t obj-intel64/bp_btfn.so -- test/astar test/small.data

