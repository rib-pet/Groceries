# Groceries
MFC C++, C++/CLR, WPF control, C# 


it's a traditional MFC mvc project which contains class CGroceriesView, CGroceriesFrame, CGroceriesDoc
CGroceriesDoc : it's used to store data
CGroceriesView: it's for UI customization
CGroceriesFrame: it's message route and other message commands and so on

Introduction:
for this project, it has 3 projects 
1. Groceries ---> MFC project which is used to US presentation.
2. PDC -> it's used to combined different model as I separate UI and data layer.
3. UserControl is a wpf project, which is replaced a View page(docking window) in Project "Groceries", you can see I leverage the C++/CLR and manage C++ technical to handle WPF (C#) and C++ mfc
