
import pandas as pd
import statsmodels.stats.contingency_tables as tbl
home = pd.read_excel(r'C:/Users/85114/Desktop/机器学习/卡方检验/home_income.xlsx')
#建立交叉表
a=pd.crosstab(home.Ts9,home.O1)
print(a)
table=tbl.Table(a)
#卡方检验
res=table.test_nominal_association()   
print('卡方值',res.statistic)  
print('p值',res.pvalue)
print('自由度',res.df)
