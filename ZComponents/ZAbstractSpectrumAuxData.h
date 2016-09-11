//================================================
#ifndef ZABSTRACTSPECTRUMAUXDATA_H
#define ZABSTRACTSPECTRUMAUXDATA_H
//================================================

//================================================
/*!
 \brief

*/
class ZAbstractSpectrumAuxData
{
public:
    /*!
     \brief

    */
    enum DataType {DT_SPE};
    /*!
     \brief

     \param type
    */
    explicit ZAbstractSpectrumAuxData(DataType type);

    /*!
     \brief

     \return DataType
    */
    DataType zp_type() const;

private:

    // VARS

    DataType zv_type; /*!< TODO: describe */

};
//================================================
#endif // ZABSTRACTSPECTRUMAUXDATA_H
