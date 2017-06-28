##############################
#       Test Stats Plot      #
##############################
data <- read.csv("../data/data2_results.txt", header=TRUE)

png("data2_plot.png", width=1000, height=625)

colorReal <- "darkgreen"
colorPredicted <- "orange"
colorDiff <- "red"

numberPoints <- length(data[, 1])
realValues <- head(data[, 1], n=numberPoints)
predictedValues <- head(data[, 2], n=numberPoints)
diffValues <- head(data[, 3], n=numberPoints)

plot(1:numberPoints, realValues, type="l", col=colorReal, xlab="Time (x)", ylab="y = 3x + 7", log="y")
title(main="Prediction of Function y = 3x + 7")
lines(1:numberPoints, predictedValues, type="l", col=colorPredicted)
lines(1:numberPoints, diffValues, type="p", col=colorDiff)
legend("topleft",
       legend=c("RealY", "PredictedY", "| RealY - PredictedY |"),
       lty=c(1, 1, 0), pch=c(NA, NA, 1), col=c(colorReal, colorPredicted, colorDiff))

