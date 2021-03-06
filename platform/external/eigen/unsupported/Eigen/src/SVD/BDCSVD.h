// research report written by Ming Gu and Stanley C.Eisenstat
// Copyright (C) 2013 Gauthier Brun <brun.gauthier@gmail.com>
// Copyright (C) 2013 Nicolas Carre <nicolas.carre@ensimag.fr>
// Copyright (C) 2013 Jean Ceccato <jean.ceccato@ensimag.fr>
// Copyright (C) 2013 Pierre Zoppitelli <pierre.zoppitelli@ensimag.fr>
// Public License v. 2.0. If a copy of the MPL was not distributed

#ifndef EIGEN_BDCSVD_H
#define EIGEN_BDCSVD_H

#define EPSILON 0.0000000000000001

#define ALGOSWAP 32

namespace Eigen {
template<typename _MatrixType> 
class BDCSVD : public SVDBase<_MatrixType>
{
  typedef SVDBase<_MatrixType> Base;
    
public:
  using Base::rows;
  using Base::cols;
  
  typedef _MatrixType MatrixType;
  typedef typename MatrixType::Scalar Scalar;
  typedef typename NumTraits<typename MatrixType::Scalar>::Real RealScalar;
  typedef typename MatrixType::Index Index;
  enum {
    RowsAtCompileTime = MatrixType::RowsAtCompileTime, 
    ColsAtCompileTime = MatrixType::ColsAtCompileTime, 
    DiagSizeAtCompileTime = EIGEN_SIZE_MIN_PREFER_DYNAMIC(RowsAtCompileTime, ColsAtCompileTime), 
    MaxRowsAtCompileTime = MatrixType::MaxRowsAtCompileTime, 
    MaxColsAtCompileTime = MatrixType::MaxColsAtCompileTime, 
    MaxDiagSizeAtCompileTime = EIGEN_SIZE_MIN_PREFER_FIXED(MaxRowsAtCompileTime, MaxColsAtCompileTime), 
    MatrixOptions = MatrixType::Options
  };

  typedef Matrix<Scalar, RowsAtCompileTime, RowsAtCompileTime, 
		 MatrixOptions, MaxRowsAtCompileTime, MaxRowsAtCompileTime>
  MatrixUType;
  typedef Matrix<Scalar, ColsAtCompileTime, ColsAtCompileTime, 
		 MatrixOptions, MaxColsAtCompileTime, MaxColsAtCompileTime>
  MatrixVType;
  typedef typename internal::plain_diag_type<MatrixType, RealScalar>::type SingularValuesType;
  typedef typename internal::plain_row_type<MatrixType>::type RowType;
  typedef typename internal::plain_col_type<MatrixType>::type ColType;
  typedef Matrix<Scalar, Dynamic, Dynamic> MatrixX;
  typedef Matrix<RealScalar, Dynamic, Dynamic> MatrixXr;
  typedef Matrix<RealScalar, Dynamic, 1> VectorType;

  BDCSVD()
    : SVDBase<_MatrixType>::SVDBase(), 
      algoswap(ALGOSWAP)
  {}


  BDCSVD(Index rows, Index cols, unsigned int computationOptions = 0)
    : SVDBase<_MatrixType>::SVDBase(), 
      algoswap(ALGOSWAP)
  {
    allocate(rows, cols, computationOptions);
  }

  BDCSVD(const MatrixType& matrix, unsigned int computationOptions = 0)
    : SVDBase<_MatrixType>::SVDBase(), 
      algoswap(ALGOSWAP)
  {
    compute(matrix, computationOptions);
  }

  ~BDCSVD() 
  {
  }
  SVDBase<MatrixType>& compute(const MatrixType& matrix, unsigned int computationOptions);

  SVDBase<MatrixType>& compute(const MatrixType& matrix)
  {
    return compute(matrix, this->m_computationOptions);
  }

  void setSwitchSize(int s) 
  {
    eigen_assert(s>3 && "BDCSVD the size of the algo switch has to be greater than 4");
    algoswap = s;
  }


  template<typename Rhs>
  inline const internal::solve_retval<BDCSVD, Rhs>
  solve(const MatrixBase<Rhs>& b) const
  {
    eigen_assert(this->m_isInitialized && "BDCSVD is not initialized.");
    eigen_assert(SVDBase<_MatrixType>::computeU() && SVDBase<_MatrixType>::computeV() && 
		 "BDCSVD::solve() requires both unitaries U and V to be computed (thin unitaries suffice).");
    return internal::solve_retval<BDCSVD, Rhs>(*this, b.derived());
  }

 
  const MatrixUType& matrixU() const
  {
    eigen_assert(this->m_isInitialized && "SVD is not initialized.");
    if (isTranspose){
      eigen_assert(this->computeV() && "This SVD decomposition didn't compute U. Did you ask for it?");
      return this->m_matrixV;
    }
    else 
    {
      eigen_assert(this->computeU() && "This SVD decomposition didn't compute U. Did you ask for it?");
      return this->m_matrixU;
    }
     
  }


  const MatrixVType& matrixV() const
  {
    eigen_assert(this->m_isInitialized && "SVD is not initialized.");
    if (isTranspose){
      eigen_assert(this->computeU() && "This SVD decomposition didn't compute V. Did you ask for it?");
      return this->m_matrixU;
    }
    else
    {
      eigen_assert(this->computeV() && "This SVD decomposition didn't compute V. Did you ask for it?");
      return this->m_matrixV;
    }
  }
 
private:
  void allocate(Index rows, Index cols, unsigned int computationOptions);
  void divide (Index firstCol, Index lastCol, Index firstRowW, 
	       Index firstColW, Index shift);
  void deflation43(Index firstCol, Index shift, Index i, Index size);
  void deflation44(Index firstColu , Index firstColm, Index firstRowW, Index firstColW, Index i, Index j, Index size);
  void deflation(Index firstCol, Index lastCol, Index k, Index firstRowW, Index firstColW, Index shift);
  void copyUV(MatrixXr naiveU, MatrixXr naiveV, MatrixX householderU, MatrixX houseHolderV);

protected:
  MatrixXr m_naiveU, m_naiveV;
  MatrixXr m_computed;
  Index nRec;
  int algoswap;
  bool isTranspose, compU, compV;
  
}; 


template<typename MatrixType>
void BDCSVD<MatrixType>::allocate(Index rows, Index cols, unsigned int computationOptions)
{
  isTranspose = (cols > rows);
  if (SVDBase<MatrixType>::allocate(rows, cols, computationOptions)) return;
  m_computed = MatrixXr::Zero(this->m_diagSize + 1, this->m_diagSize );
  if (isTranspose){
    compU = this->computeU();
    compV = this->computeV();    
  } 
  else
  {
    compV = this->computeU();
    compU = this->computeV();   
  }
  if (compU) m_naiveU = MatrixXr::Zero(this->m_diagSize + 1, this->m_diagSize + 1 );
  else m_naiveU = MatrixXr::Zero(2, this->m_diagSize + 1 );
  
  if (compV) m_naiveV = MatrixXr::Zero(this->m_diagSize, this->m_diagSize);
  

  
  if (isTranspose){
    bool aux;
    if (this->computeU()||this->computeV()){
      aux = this->m_computeFullU;
      this->m_computeFullU = this->m_computeFullV;
      this->m_computeFullV = aux;
      aux = this->m_computeThinU;
      this->m_computeThinU = this->m_computeThinV;
      this->m_computeThinV = aux;
    } 
  }
}

template<>
SVDBase<Matrix<int, Dynamic, Dynamic> >&
BDCSVD<Matrix<int, Dynamic, Dynamic> >::compute(const MatrixType& matrix, unsigned int computationOptions) {
  allocate(matrix.rows(), matrix.cols(), computationOptions);
  this->m_nonzeroSingularValues = 0;
  m_computed = Matrix<int, Dynamic, Dynamic>::Zero(rows(), cols());
  for (int i=0; i<this->m_diagSize; i++)   {
    this->m_singularValues.coeffRef(i) = 0;
  }
  if (this->m_computeFullU) this->m_matrixU = Matrix<int, Dynamic, Dynamic>::Zero(rows(), rows());
  if (this->m_computeFullV) this->m_matrixV = Matrix<int, Dynamic, Dynamic>::Zero(cols(), cols()); 
  this->m_isInitialized = true;
  return *this;
}


template<typename MatrixType>
SVDBase<MatrixType>&
BDCSVD<MatrixType>::compute(const MatrixType& matrix, unsigned int computationOptions) 
{
  allocate(matrix.rows(), matrix.cols(), computationOptions);
  using std::abs;

  
  MatrixType copy;
  if (isTranspose) copy = matrix.adjoint();
  else copy = matrix;
  
  internal::UpperBidiagonalization<MatrixX > bid(copy);

  
  
  MatrixXr temp;
  temp = bid.bidiagonal().toDenseMatrix().transpose();
  m_computed.setZero();
  for (int i=0; i<this->m_diagSize - 1; i++)   {
    m_computed(i, i) = temp(i, i);
    m_computed(i + 1, i) = temp(i + 1, i);
  }
  m_computed(this->m_diagSize - 1, this->m_diagSize - 1) = temp(this->m_diagSize - 1, this->m_diagSize - 1);
  divide(0, this->m_diagSize - 1, 0, 0, 0);

  
  for (int i=0; i<this->m_diagSize; i++)   {
    RealScalar a = abs(m_computed.coeff(i, i));
    this->m_singularValues.coeffRef(i) = a;
    if (a == 0){
      this->m_nonzeroSingularValues = i;
      break;
    }
    else  if (i == this->m_diagSize - 1)
    {
      this->m_nonzeroSingularValues = i + 1;
      break;
    }
  }
  copyUV(m_naiveV, m_naiveU, bid.householderU(), bid.householderV());
  this->m_isInitialized = true;
  return *this;
}


template<typename MatrixType>
void BDCSVD<MatrixType>::copyUV(MatrixXr naiveU, MatrixXr naiveV, MatrixX householderU, MatrixX householderV){
  if (this->computeU()){
    MatrixX temp = MatrixX::Zero(naiveU.rows(), naiveU.cols());
    temp.real() = naiveU;
    if (this->m_computeThinU){
      this->m_matrixU = MatrixX::Identity(householderU.cols(), this->m_nonzeroSingularValues );
      this->m_matrixU.block(0, 0, this->m_diagSize, this->m_nonzeroSingularValues) = 
	temp.block(0, 0, this->m_diagSize, this->m_nonzeroSingularValues);
      this->m_matrixU = householderU * this->m_matrixU ;
    }
    else
    {
      this->m_matrixU = MatrixX::Identity(householderU.cols(), householderU.cols());
      this->m_matrixU.block(0, 0, this->m_diagSize, this->m_diagSize) = temp.block(0, 0, this->m_diagSize, this->m_diagSize);
      this->m_matrixU = householderU * this->m_matrixU ;
    }
  }
  if (this->computeV()){
    MatrixX temp = MatrixX::Zero(naiveV.rows(), naiveV.cols());
    temp.real() = naiveV;
    if (this->m_computeThinV){
      this->m_matrixV = MatrixX::Identity(householderV.cols(),this->m_nonzeroSingularValues );
      this->m_matrixV.block(0, 0, this->m_nonzeroSingularValues, this->m_nonzeroSingularValues) = 
	temp.block(0, 0, this->m_nonzeroSingularValues, this->m_nonzeroSingularValues);
      this->m_matrixV = householderV * this->m_matrixV ;
    }
    else  
    {
      this->m_matrixV = MatrixX::Identity(householderV.cols(), householderV.cols());
      this->m_matrixV.block(0, 0, this->m_diagSize, this->m_diagSize) = temp.block(0, 0, this->m_diagSize, this->m_diagSize);
      this->m_matrixV = householderV * this->m_matrixV;
    }
  }
}


template<typename MatrixType>
void BDCSVD<MatrixType>::divide (Index firstCol, Index lastCol, Index firstRowW, 
				 Index firstColW, Index shift)
{
  
  using std::pow;
  using std::sqrt;
  using std::abs;
  const Index n = lastCol - firstCol + 1;
  const Index k = n/2;
  RealScalar alphaK;
  RealScalar betaK; 
  RealScalar r0; 
  RealScalar lambda, phi, c0, s0;
  MatrixXr l, f;
  
  
  if (n < algoswap){
    JacobiSVD<MatrixXr> b(m_computed.block(firstCol, firstCol, n + 1, n), 
			  ComputeFullU | (ComputeFullV * compV)) ;
    if (compU) m_naiveU.block(firstCol, firstCol, n + 1, n + 1).real() << b.matrixU();
    else 
    {
      m_naiveU.row(0).segment(firstCol, n + 1).real() << b.matrixU().row(0);
      m_naiveU.row(1).segment(firstCol, n + 1).real() << b.matrixU().row(n);
    }
    if (compV) m_naiveV.block(firstRowW, firstColW, n, n).real() << b.matrixV();
    m_computed.block(firstCol + shift, firstCol + shift, n + 1, n).setZero();
    for (int i=0; i<n; i++)
    {
      m_computed(firstCol + shift + i, firstCol + shift +i) = b.singularValues().coeffRef(i);
    }
    return;
  }
  
  alphaK =  m_computed(firstCol + k, firstCol + k);
  betaK = m_computed(firstCol + k + 1, firstCol + k);
  
  
  
  divide(k + 1 + firstCol, lastCol, k + 1 + firstRowW, k + 1 + firstColW, shift);
  divide(firstCol, k - 1 + firstCol, firstRowW, firstColW + 1, shift + 1);
  if (compU)
  {
    lambda = m_naiveU(firstCol + k, firstCol + k);
    phi = m_naiveU(firstCol + k + 1, lastCol + 1);
  } 
  else 
  {
    lambda = m_naiveU(1, firstCol + k);
    phi = m_naiveU(0, lastCol + 1);
  }
  r0 = sqrt((abs(alphaK * lambda) * abs(alphaK * lambda))
	    + abs(betaK * phi) * abs(betaK * phi));
  if (compU)
  {
    l = m_naiveU.row(firstCol + k).segment(firstCol, k);
    f = m_naiveU.row(firstCol + k + 1).segment(firstCol + k + 1, n - k - 1);
  } 
  else 
  {
    l = m_naiveU.row(1).segment(firstCol, k);
    f = m_naiveU.row(0).segment(firstCol + k + 1, n - k - 1);
  }
  if (compV) m_naiveV(firstRowW+k, firstColW) = 1;
  if (r0 == 0)
  {
    c0 = 1;
    s0 = 0;
  }
  else
  {
    c0 = alphaK * lambda / r0;
    s0 = betaK * phi / r0;
  }
  if (compU)
  {
    MatrixXr q1 (m_naiveU.col(firstCol + k).segment(firstCol, k + 1));     
    
    for (Index i = firstCol + k - 1; i >= firstCol; i--) 
    {
      m_naiveU.col(i + 1).segment(firstCol, k + 1) << m_naiveU.col(i).segment(firstCol, k + 1);
    }
    
    m_naiveU.col(firstCol).segment( firstCol, k + 1) << (q1 * c0);
    
    m_naiveU.col(lastCol + 1).segment(firstCol, k + 1) << (q1 * ( - s0));
    
    m_naiveU.col(firstCol).segment(firstCol + k + 1, n - k) << 
      m_naiveU.col(lastCol + 1).segment(firstCol + k + 1, n - k) *s0; 
    
    m_naiveU.col(lastCol + 1).segment(firstCol + k + 1, n - k) *= c0; 
  } 
  else 
  {
    RealScalar q1 = (m_naiveU(0, firstCol + k));
    
    for (Index i = firstCol + k - 1; i >= firstCol; i--) 
    {
      m_naiveU(0, i + 1) = m_naiveU(0, i);
    }
    
    m_naiveU(0, firstCol) = (q1 * c0);
    
    m_naiveU(0, lastCol + 1) = (q1 * ( - s0));
    
    m_naiveU(1, firstCol) = m_naiveU(1, lastCol + 1) *s0; 
    
    m_naiveU(1, lastCol + 1) *= c0;
    m_naiveU.row(1).segment(firstCol + 1, k).setZero();
    m_naiveU.row(0).segment(firstCol + k + 1, n - k - 1).setZero();
  }
  m_computed(firstCol + shift, firstCol + shift) = r0;
  m_computed.col(firstCol + shift).segment(firstCol + shift + 1, k) << alphaK * l.transpose().real();
  m_computed.col(firstCol + shift).segment(firstCol + shift + k + 1, n - k - 1) << betaK * f.transpose().real();


  
  
  

  deflation(firstCol, lastCol, k, firstRowW, firstColW, shift);

  
  JacobiSVD<MatrixXr> res= JacobiSVD<MatrixXr>(m_computed.block(firstCol + shift, firstCol +shift, n + 1, n), 
					       ComputeFullU | (ComputeFullV * compV)) ;
  if (compU) m_naiveU.block(firstCol, firstCol, n + 1, n + 1) *= res.matrixU();
  else m_naiveU.block(0, firstCol, 2, n + 1) *= res.matrixU();
  
  if (compV) m_naiveV.block(firstRowW, firstColW, n, n) *= res.matrixV();
  m_computed.block(firstCol + shift, firstCol + shift, n, n) << MatrixXr::Zero(n, n);
  for (int i=0; i<n; i++)
    m_computed(firstCol + shift + i, firstCol + shift +i) = res.singularValues().coeffRef(i);
  


}


template <typename MatrixType>
void BDCSVD<MatrixType>::deflation43(Index firstCol, Index shift, Index i, Index size){
  using std::abs;
  using std::sqrt;
  using std::pow;
  RealScalar c = m_computed(firstCol + shift, firstCol + shift);
  RealScalar s = m_computed(i, firstCol + shift);
  RealScalar r = sqrt(pow(abs(c), 2) + pow(abs(s), 2));
  if (r == 0){
    m_computed(i, i)=0;
    return;
  }
  c/=r;
  s/=r;
  m_computed(firstCol + shift, firstCol + shift) = r;  
  m_computed(i, firstCol + shift) = 0;
  m_computed(i, i) = 0;
  if (compU){
    m_naiveU.col(firstCol).segment(firstCol,size) = 
      c * m_naiveU.col(firstCol).segment(firstCol, size) - 
      s * m_naiveU.col(i).segment(firstCol, size) ;

    m_naiveU.col(i).segment(firstCol, size) = 
      (c + s*s/c) * m_naiveU.col(i).segment(firstCol, size) + 
      (s/c) * m_naiveU.col(firstCol).segment(firstCol,size);
  }
}


template <typename MatrixType>
void BDCSVD<MatrixType>::deflation44(Index firstColu , Index firstColm, Index firstRowW, Index firstColW, Index i, Index j, Index size){
  using std::abs;
  using std::sqrt;
  using std::conj;
  using std::pow;
  RealScalar c = m_computed(firstColm, firstColm + j - 1);
  RealScalar s = m_computed(firstColm, firstColm + i - 1);
  RealScalar r = sqrt(pow(abs(c), 2) + pow(abs(s), 2));
  if (r==0){
    m_computed(firstColm + i, firstColm + i) = m_computed(firstColm + j, firstColm + j);
    return;
  }
  c/=r;
  s/=r;
  m_computed(firstColm + i, firstColm) = r;  
  m_computed(firstColm + i, firstColm + i) = m_computed(firstColm + j, firstColm + j);
  m_computed(firstColm + j, firstColm) = 0;
  if (compU){
    m_naiveU.col(firstColu + i).segment(firstColu, size) = 
      c * m_naiveU.col(firstColu + i).segment(firstColu, size) - 
      s * m_naiveU.col(firstColu + j).segment(firstColu, size) ;

    m_naiveU.col(firstColu + j).segment(firstColu, size) = 
      (c + s*s/c) *  m_naiveU.col(firstColu + j).segment(firstColu, size) + 
      (s/c) * m_naiveU.col(firstColu + i).segment(firstColu, size);
  } 
  if (compV){
    m_naiveV.col(firstColW + i).segment(firstRowW, size - 1) = 
      c * m_naiveV.col(firstColW + i).segment(firstRowW, size - 1) + 
      s * m_naiveV.col(firstColW + j).segment(firstRowW, size - 1) ;

    m_naiveV.col(firstColW + j).segment(firstRowW, size - 1)  = 
      (c + s*s/c) * m_naiveV.col(firstColW + j).segment(firstRowW, size - 1) - 
      (s/c) * m_naiveV.col(firstColW + i).segment(firstRowW, size - 1);
  }
}



template <typename MatrixType>
void BDCSVD<MatrixType>::deflation(Index firstCol, Index lastCol, Index k, Index firstRowW, Index firstColW, Index shift){
  
  RealScalar EPS = EPSILON * (std::max<RealScalar>(m_computed(firstCol + shift + 1, firstCol + shift + 1), m_computed(firstCol + k, firstCol + k)));
  const Index length = lastCol + 1 - firstCol;
  if (m_computed(firstCol + shift, firstCol + shift) < EPS){
    m_computed(firstCol + shift, firstCol + shift) = EPS;
  }
  
  for (Index i=firstCol + shift + 1;i<=lastCol + shift;i++){
    if (std::abs(m_computed(i, firstCol + shift)) < EPS){
      m_computed(i, firstCol + shift) = 0;
    }
  }

  
  for (Index i=firstCol + shift + 1;i<=lastCol + shift; i++){
    if (m_computed(i, i) < EPS){
      deflation43(firstCol, shift, i, length);
    }
  }

  
 
  Index i=firstCol + shift + 1, j=firstCol + shift + k + 1;
  
  Index *permutation = new Index[length];

  for (Index p =1; p < length; p++) {
    if (i> firstCol + shift + k){
      permutation[p] = j;
      j++;
    } else if (j> lastCol + shift) 
    {
      permutation[p] = i;
      i++;
    }
    else 
    {
      if (m_computed(i, i) < m_computed(j, j)){
        permutation[p] = j;
        j++;
      } 
      else
      {
        permutation[p] = i;
        i++;
      }
    }
  }
  
  RealScalar aux;
  
  
  Index *realInd = new Index[length];
  Index *realCol = new Index[length];
  for (int pos = 0; pos< length; pos++){
    realCol[pos] = pos + firstCol + shift;
    realInd[pos] = pos;
  }
  const Index Zero = firstCol + shift;
  VectorType temp;
  for (int i = 1; i < length - 1; i++){
    const Index I = i + Zero;
    const Index realI = realInd[i];
    const Index j  = permutation[length - i] - Zero;
    const Index J = realCol[j];
    
    
    aux = m_computed(I, I); 
    m_computed(I, I) = m_computed(J, J);
    m_computed(J, J) = aux;
    
    
    aux = m_computed(I, Zero); 
    m_computed(I, Zero) = m_computed(J, Zero);
    m_computed(J, Zero) = aux;

    
    if (compU) {
      temp = m_naiveU.col(I - shift).segment(firstCol, length + 1);
      m_naiveU.col(I - shift).segment(firstCol, length + 1) << 
        m_naiveU.col(J - shift).segment(firstCol, length + 1);
      m_naiveU.col(J - shift).segment(firstCol, length + 1) << temp;
    } 
    else
    {
      temp = m_naiveU.col(I - shift).segment(0, 2);
      m_naiveU.col(I - shift).segment(0, 2) << 
        m_naiveU.col(J - shift).segment(0, 2);
      m_naiveU.col(J - shift).segment(0, 2) << temp;      
    }
    if (compV) {
      const Index CWI = I + firstColW - Zero;
      const Index CWJ = J + firstColW - Zero;
      temp = m_naiveV.col(CWI).segment(firstRowW, length);
      m_naiveV.col(CWI).segment(firstRowW, length) << m_naiveV.col(CWJ).segment(firstRowW, length);
      m_naiveV.col(CWJ).segment(firstRowW, length) << temp;
    }

    
    realCol[realI] = J;
    realCol[j] = I;
    realInd[J - Zero] = realI;
    realInd[I - Zero] = j;
  }
  for (Index i = firstCol + shift + 1; i<lastCol + shift;i++){
    if ((m_computed(i + 1, i + 1) - m_computed(i, i)) < EPS){
      deflation44(firstCol , 
		  firstCol + shift, 
		  firstRowW, 
		  firstColW, 
		  i - Zero, 
		  i + 1 - Zero, 
		  length);
    }
  }
  delete [] permutation;
  delete [] realInd;
  delete [] realCol;

}


namespace internal{

template<typename _MatrixType, typename Rhs>
struct solve_retval<BDCSVD<_MatrixType>, Rhs>
  : solve_retval_base<BDCSVD<_MatrixType>, Rhs>
{
  typedef BDCSVD<_MatrixType> BDCSVDType;
  EIGEN_MAKE_SOLVE_HELPERS(BDCSVDType, Rhs)

  template<typename Dest> void evalTo(Dest& dst) const
  {
    eigen_assert(rhs().rows() == dec().rows());
    
    
    Index diagSize = (std::min)(dec().rows(), dec().cols());
    typename BDCSVDType::SingularValuesType invertedSingVals(diagSize);
    Index nonzeroSingVals = dec().nonzeroSingularValues();
    invertedSingVals.head(nonzeroSingVals) = dec().singularValues().head(nonzeroSingVals).array().inverse();
    invertedSingVals.tail(diagSize - nonzeroSingVals).setZero();
    
    dst = dec().matrixV().leftCols(diagSize)
      * invertedSingVals.asDiagonal()
      * dec().matrixU().leftCols(diagSize).adjoint()
      * rhs();	
    return;
  }
};

} 


} 

#endif
