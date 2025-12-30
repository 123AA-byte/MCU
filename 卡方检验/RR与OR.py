#SciPy实现OR
import pandas as pd
import statsmodels.stats.contingency_tables as tbl
home = pd.read_excel(r'C:/Users/85114/Desktop/机器学习/卡方检验/home_income.xlsx')
#收入级别与有无轿车的交叉表
a=pd.crosstab(home.Ts9,home.O1)
print(a)
#scipy的实现
from scipy import stats as ss
b=a.values#必须转换为数组，不能是DATAFRAME的格式
OR,P=ss.fisher_exact(b)  #p值小于0.05，拒绝原假设，认为两个变量相关
print('OR:',OR)
print('P:',P)
#statsmodels的实现b
table=tbl.Table2x2(b)
c=table.oddsratio  #OR值
print('OR:',c)
d=table.summary()  #卡方检验结果
print(d)