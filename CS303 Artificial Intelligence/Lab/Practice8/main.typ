#import "template.typ": *

#show: project.with(
  title: "AI Practice 8",
  authors: (
    (
      name: "陈贲(12212231)",
    ),
  ),
)

*Background*: You want to predict whether movies will be profitable based on their screenplays. You hire two
critics, A and B, to read a script and rate it on a scale of 1 to 5. The critics are not perfect; here are five data
points including the critics' scores and the performance of the movie:

#show table.cell.where(y: 0): strong
#set table(
  stroke: (x, y) => if y == 0 {
    (bottom: 1pt + black)
  }
)

#align(center, table(
  columns: 4,
  align: center,
  table.header([Movie Name], [A Score], [B Score], [Profit?]),
  [Pellet Power], [1], [1], [No],
  [Ghosts!], [3], [2], [Yes],
  [Pac is Bac], [4], [5], [No],
  [Not a Pizza], [3], [4], [Yes],
  [Endless Maze], [2], [3], [Yes]
))

#problem[
  - Plot the data points on a 2D plane, label profitable movies with '+' and non-profitable movies with '-', and determine if the data are linearly separable.
  - Analyze which model, SVM, NN, or DT, might be more suitable in this situation and explain why.
]

#solution[
  #figure(image("plot.svg", width: 80%))
  Based on the visualization of the data points, the data are not linearly separable. Since the dataset is not large enought for deep learning, the neural network model is not suitable. The decision tree model is not suitable either because it is not good at handling non-linear data and will probably miss the corelation between A score and B score. Therefore, the SVM model is more suitable with the kernel trick to handle the non-linear data.
]

#problem[
  - Train a perceptron model using the given scores as features, including a bias feature (f0=1, f1=A's score, f2=B's score).
  - Describe how to update the weights and provide the weights after the first update.
  - Discuss whether the perceptron model can perfectly classify the following scenarios with the given features and explain why:
    - (i) If the total of their scores is more than 8, the movie will definitely be a success; otherwise, it will fail.
    - (ii) The movie will succeed if and only if each critic gives either a score of 2 or a score of 3.
    - (iii) The movie will succeed if and only if both critics agree.
]

#solution[
We have a perceptron model that $ f(x) = sum_(i=0)^2 w_i * x_i, x_i = [1, "A's score", "B's score"] $

Firstly, initialize the weights as $w = [0, 0, 0]$. And then update the weights and bias if classification fails using the following formula:
$ w = w + y_i * x_i $
where $y_i = 1$ if the movie is profitable and $y_i = -1$ if the movie is not profitable.

So after first iteration, the weights will be $w = [1, 3, 2]$.

Under each scenario:
 - (i) Perfectly classified, since the overall score of 8 makes an another plane with $f_1 + f_2 - 8 = 0$, so it's linearly separable.
 - (ii) Imperfectly classified, since the scenarios draw a rectangle in the plane, which is not linearly separable.
 - (iii) Perfectly classified, since the scenario is linearly separable with a line $f_1 - f_2 = 0$.
]

#problem[
 - Given that the data is not linearly separable, apply a soft margin SVM to classify the data.
 - Explain the role of the regularization parameter C in the context of soft margin SVM and how it affects the decision boundary.
 - Discuss the trade-off between maximizing the margin and minimizing the classification error in the context of soft margin SVM.
]

#solution[
  The regularization parameter $C$ in a soft margin SVM controls the trade-off between maximizing the margin and minimizing the classification error.
  - Higher $C$ causes larger penalty for misclassification, it will try to classify all the training points correctly and the boundary will become complex. The model may sacrifice a larger margin to ensure that fewer points are misclassified, which can lead to overfitting.
  - With lower $C$, the SVM will increase the soft margin between the two classes. This allows the model to tolerate more classification errors in exchange for a more generalized model. 
  In the context of a soft margin SVM, the trade-off between maximizing the margin and minimizing classification error is governed by the parameter $C$.
  - Maximizing the Margin: requires lower $C$ and the decision boundary will be smoother and more tolerant of misclassified points, reflecting a broader margin.
  - Minimizing Classification Error: requires higher $C$ and the decision boundary will attempt to separate the classes with fewer misclassifications.
]

#problem[
 - Build a decision tree model using the same dataset and discuss how to choose the appropriate splitting features.
 - Analyze the impact of the depth of the decision tree on model performance and discuss how pruning can prevent overfitting.
]

#solution[
  To build a decision tree model, we start from the root node and choose one of the five in A score and then B score as the splitting feature, so there are 25 leaf nodes in total. At each node, we calculate the Gini impurity of the dataset and choose the feature that minimizes the impurity as the splitting feature. When reaches the leaves, we assign a label to the leaf node based on the majority class of the data points in the node.

  The depth could increase the model complexity exponentially and may lead to overfitting, since DT performs a greedy search over the feature space. Pruning helps reduce model complexity, making the decision tree less likely to fit to random noise in the training data. Also, pruning can help to terminate the growth of the tree earlier, which can prevent over-searching the feature space and reduce the risk of overfitting.
]

#problem[
  - Compare the advantages and disadvantages of SVM, NN, and DT in handling this dataset.
  - Provide a comprehensive evaluation, explaining which model to choose in practical applications and the reasons for choosing it.
]

#solution[

#table(
  columns: 3,
  align: center,
  table.header([Model], [Advantages], [Disadvantages]),
  [SVM], [Better accuracy and better fit for small datasets], [Hard to tune and select the right kernel],
  [NN], [Tolerance to noisy data], [Insufficient data leads to underfitting],
  [DT], [Better to understand and interpret], [Ignores the correlation between features]
)

  In this case, the SVM model with soft margin and non-linear kernel is the best choice. The dataset is small and the SVM model can handle non-linear data well. The regularization parameter $C$ can be tuned to balance the margin and classification error. The SVM model can provide a good balance between accuracy and generalization, making it a suitable choice for practical applications.
]
