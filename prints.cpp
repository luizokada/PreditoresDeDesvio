//print do update

cout << "Antes BHR\n"
     << br.taken << "\n";
for (int i = BHR.size() - 1; i >= 0; i--)
{
    int num = BHR[i] ? 1 : 0;
    cout << num << " ";
}
cout << "\n--------------------------------------\n";
//update

cout << "DEPOIS BHR\n";
for (int i = BHR.size() - 1; i >= 0; i--)
{
    int num = BHR[i] ? 1 : 0;
    cout << num << " ";
}
cout << "\n--------------------------------------\n";

//Print INDEX
cout << "--------------------------------\n"
     << ""
        "ANTES\n"
     << "BHR : ";
for (int i = 0; i < BHR.size(); i++)
{
    cout << BHR[i];
}

cout << "\nindex\n"
     << index << "\n";
cout << "ESTADO" << PHT[index].estado << "\n";

taken = isTaken(PHT[index]);
cout << "Taken" << taken << "\n";

//update
cout << "DEPOIS\n"
     << "BHR : ";
for (int i = 0; i < BHR.size(); i++)
{
    cout << BHR[i];
}

cout << "Taken" << br.taken << "\n";
cout << "\nESTADO" << PHT[index].estado << "\n";
cout << "--------------------------------------\n";