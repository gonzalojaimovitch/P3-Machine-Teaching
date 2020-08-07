setwd("/home/cesar/Dropbox/MT/")
library(plyr)


#### Functions!
compsize<-function(s)
{
  kk<-gsub("\\(|\\)|\'| ", "", s)
  v<-strsplit(kk,"[,]")
  return(unlist(t(sapply(v,nchar)))[1,])
}

compelias<-function(el)
{
  return(sum(sapply(compsize(el),elias)))
}

elias<-function(x)
{
  r<-0
  if(x==0)     r<-1
  else if(x==1)  r<-4
  else if(x==2)  r<-5
  else if(x==3)  r<-8
  else if(x==4)  r<-9
  else if(x==5)  r<-10
  else if(x==6)  r<-11
  else if(x==7)  r<-14
  else if(x==8)  r<-15
  else if(x==9)  r<-16
  else if(x==10)  r<-17
  else r<--1
  
  return(r)
}

transfs<-function(stri)
{
  x<-gsub("\\[|\\]", "", stri)
  x<-strsplit(x, "[,]")
  return (sum(as.integer((unlist(x)))))
}

accumprob<-function(i,j)
{
  vec<-c(i:j)
  return (sum (2^-vec))
}

transfl<-function(stri)
{
  x<-gsub("\\[|\\]", "", stri)
  x<-strsplit(x, "[,]")
  return (length(as.integer((unlist(x)))))
}




### TS
dat<-read.csv("P3Python/results/res7.txt",stringsAsFactors =FALSE,header = FALSE,colClasses = "character",sep=":")
dat<-dat[-c(1,2),]## remove first two lines


names(dat)<-c("id","ws","program","sizes")
dat[,3]<-substring(dat[,3], 2) #remove extra space at the beginning
dat["TS"]=sapply(dat[,4],transfs)
dat["TD"]=sapply(dat[,4],transfl)
dat["lengthprog"]=nchar(dat[,3])

dat["eliasTS"]<-sapply(dat[,"ws"],compelias)


datfr<-ddply(dat,~program,summarise,freq=length(program)) ## summarise by program

#datn<-dat[order(dat$eliasTS),]#order by Elias
datn<-dat

dats<-datn[!duplicated(datn$program),] ### remove duplicated programs, we only take the first time it appears

dfs<-join(x=dats,y=datfr,by = "program",type="inner") ## join selecting the short ws




pdf("plots/scatter.pdf")
plot(dfs[,"TS"], dfs[,"lengthprog"], main="Scatterplot", 
     xlab="TS ", ylab="lengthprog ", pch=19)
dev.off()

#pdf("plots/density.pdf")
#d<-density(dfs[,"lengthprog"])
#plot(d)
#dev.off()

pdf("plots/histTS.pdf")
hist(dfs[,"lengthprog"],xlab="Length of programs",ylim=c(0,1300),xlim=c(0,15), main="")
dev.off()

pdf("plots/histTS_TD.pdf")
hist(dfs[,"TD"],xlab="Teaching Dimension",  main=paste("Histogram of Teaching Dimension for", expression(ws>=1)))
dev.off()

pdf("plots/sizevslprgTS.pdf")
plot(dfs$TS,dfs$lengthprog,xlab="Witness size (number of alphabet symbols)",ylab="Program Length (number of instructions)",xlim = c(0,10) , ylim= c(1,15), 
     col=rgb(0,0,0,alpha=0.1),cex=0.8 ,pch=20)

dev.off()


### Size of the circle in the scatter plot proportional to frequency
library(plyr)
resumen<-ddply(dfs,.(TS,lengthprog),nrow)
itvl <- c(1,300,800)
itvs <- c(5,15,32)
pdf("plots/sizevslprgTS.pdf")
symbols(x=resumen$TS, y=resumen$lengthprog, circles=(resumen$V1+120), inches=1/4,xlim = c(0,10) , ylim= c(1,15),
        bg="blue2", xlab="Witness size (number of alphabet symbols)",ylab="Program Length (number of instructions)")
legend(x = c(0.6, 2.1), y = c(8.8,13.8), legend=itvl, pt.cex=itvs/5, pch=1,col  =  "blue2",y.intersp=2,x.intersp=1.5,bty = "n" )
dev.off()

#pdf("plots/histlengthTS.pdf")
#hist(dfs$TS, xlab="Witness size (Elias coding)",main="")
#dev.off()

pdf("plots/histBitsTS.pdf")
hist(dfs$TS, xlab="Witness size (symbols of the alphabet)",main="")
dev.off()


write.table(dfs,file="filteredprogsTS7size.txt",na="",row.names=FALSE,quote=FALSE,sep=",")


expectedcost<-2^-as.numeric(rownames(dfs))*dfs$TS
diste<-table(dfs$TS)
#######################################################################

### TD 1
#######################################################################

datd<-read.csv("resp_prog9b.txt",stringsAsFactors =FALSE,header = FALSE,colClasses = "character")

datd[,"V4"]<-as.double(datd[,"V4"])
datd[,"V1"]<-gsub('\\.','',datd[,1])
datd$lenI<-nchar(datd$V1)
datd$lenO<-nchar(datd$V2)
datd$lenP<-nchar(datd$V3)

datd["eliasTD"]<-sapply(datd$lenI,elias)+sapply(datd$lenO,elias)



datfrec<-ddply(datd,~V3,summarise,freq=length(V3)) ##summarise per program

#dato<-datd[order(datd$eliasTD),]#order by Elias
dato<-datd

datf<-dato[!duplicated(dato$V3),] # select the first fo each program


#df<-data.frame(input=datf$V1,output=datf$V2,prog=datf$V3,freq=datfrec$freq,steps=datfrec$mean)
df<-join(x=datf,y=datfrec,by = "V3")

names(df)<-c("input","output","prog","steps","lenInput","lenOutput","lenprg","eliasTD","freq")
df<-df[2:length(df[,1]),]## remove empty program
write.table(df,file="filteredprogs9bsize.txt",na="",row.names=FALSE,quote=FALSE,sep=",")

#nd<-merge(df,dat,by.x = "prog",by.y="program")

pdf("plots/scatterTD1.pdf")
plot(df[,"lenInput"]+df[,"lenOutput"], df[,"lenprg"], main="Scatterplot", 
     xlab="lenI+lenO ", ylab="lengthprog ", pch=19)
dev.off()

#pdf("plots/densityTD1.pdf")
#d<-density(df[,"lenprg"])
#plot(d)
#dev.off()

pdf("plots/histTD.pdf")
hist(df[,"lenprg"],xlab="Length of programs", ylim=c(0,1300),xlim=c(0,15),main="")
dev.off()

pdf("plots/sizevslprgTD.pdf")
plot(df$lenInput+df$lenOutput,df$lenprg,xlab="Witness size (number of alphabet symbols)",ylab="Program Length (number of instructions)",xlim = c(0,10) , ylim= c(1,15), 
     col=rgb(0,0,0,alpha=0.1),cex=0.8 ,pch=20)
dev.off()

### Size of the circle in the scatter plot proportional to frequency
library(plyr)
df$wslength<-df$lenInput+df$lenOutput
resumen<-ddply(df,.(wslength,lenprg),nrow)
itvl <- c(1,100,350)
itvs <- c(5,15,25)
pdf("plots/sizevslprgTD.pdf")
symbols(x=resumen$wslength, y=resumen$lenprg, circles=(resumen$V1+70), inches=1/5,xlim = c(0,10) , ylim= c(1,15),
        bg="blue2", xlab="Witness size (number of alphabet symbols)",ylab="Program Length (number of instructions)")
legend(x = c(0.6, 2.1), y = c(9.8,14.8), legend=itvl, pt.cex=itvs/5, pch=1,col  =  "blue2",y.intersp=2,x.intersp=1.5,bty = "n" )
dev.off()



totdf<-merge(df,dfs,by.x="prog",by.y = "program")
#totdf$eliasTS<-pmin(totdf$eliasTS,totdf$eliasTD)
# remove two strane cases
totdf[which(totdf$TS>(totdf$lenInput+totdf$lenOutput)),]$TS<-totdf[which(totdf$TS>(totdf$lenInput+totdf$lenOutput)),]$lenInput+totdf[which(totdf$TS>(totdf$lenInput+totdf$lenOutput)),]$lenOutput

write.table(totdf,file="allsize.txt",na="",row.names=FALSE,quote=FALSE,sep=",")

notinTD<-subset(dfs,!(dfs$program %in% df$prog)) ### Programs in TS but not in TD

pdf("plots/scattersize.pdf")
plot( totdf$lenInput+totdf$lenOutput,totdf[,"TS"],  xlim = c(0, 10),ylim = c(0, 10),
     xlab=expression(D==1) , ylab=expression(D>=1), pch=19, col=rgb(0,0,0,alpha=0.1),cex=0.8 )



#for (i in c(1:length(notinTD$program)))
#{
# if(notinTD$eliasTS[i]<18) lines(c(18,29), c(notinTD$eliasTS[i],notinTD$eliasTS[i]),col=rgb(0,0,0,alpha=0.01),cex=0.8 ) 
#  else lines(c(notinTD$eliasTS[i],29), c(notinTD$eliasTS[i],notinTD$eliasTS[i]),col=rgb(0,0,0,alpha=0.01),cex=0.8 ) 
#}

dev.off()


totdf$wslen<-totdf$lenInput+totdf$lenOutput
### Size of the circle in the scatter plot proportional to frequency
library(plyr)

resumen<-ddply(totdf,.(wslen,TS),nrow)
itvl <- c(1,100,240)
itvs <- c(4,15,25)
pdf("plots/scattersize.pdf")
symbols(x=resumen$wslen, y=resumen$TS, circles=(resumen$V1+20), inches=1/5,
        bg="blue2", xlab=expression(D==1) , ylab=expression(D>=1) )
legend(0.5,8, legend=itvl, pt.cex=itvs/5, pch=1,col  =  "blue2",y.intersp=2,x.intersp=1.5,bty = "n" )
dev.off()





library("PerformanceAnalytics")
my_data<-df[,c(4:8)]
pdf("pairs9b.pdf")
chart.Correlation(my_data, histogram=TRUE, pch=19)
dev.off()
