##############################
#       Test Stats Plot      #
##############################
data <- read.csv("data/data1_results.txt", header=TRUE)

png("data/data1_plot.png", width=800, height=500)

colorReal <- "darkgreen"
colorPredicted <- "orange"
colorDiff <- "red"

numberPoints <- 3000 #length(data[, 1])
realValues <- head(data[, 1], n=numberPoints)
predictedValues <- head(data[, 2], n=numberPoints)
diffValues <- head(data[, 3], n=numberPoints)

plot(1:numberPoints, realValues, type="l", col=colorReal, xlab="Time (t)", ylab="Temperature (ºC)")
title(main="Prediction of Hourly Air Temperature at the San Francisco International Airport\nfrom January 1st, 2015 to December 31st, 2016")
lines(1:numberPoints, predictedValues, type="l", col=colorPredicted)
lines(1:numberPoints, diffValues, type="p", col=colorDiff)
legend("topleft",
       legend=c("Real Temperature", "Predicted Temperature", "| Real - Predicted |"),
       lty=c(1, 1, 0), pch=c(NA, NA, 1), col=c(colorReal, colorPredicted, colorDiff))

