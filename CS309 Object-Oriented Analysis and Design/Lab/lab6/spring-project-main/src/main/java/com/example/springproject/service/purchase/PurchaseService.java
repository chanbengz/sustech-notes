package com.example.springproject.service.purchase;


import com.example.springproject.domain.Purchase;

import com.example.springproject.dto.PurchaseDto;
import java.util.List;

public interface PurchaseService {

    /**
     * Functionality Desc:
     *   查找所有购买记录
     * @return
     * @author Jiachen<zhangjc1999 @ gmail.com>
     * @date   2020/10/30 9:16 下午
     */
    public List<Purchase> findAll();

    /**
     * Functionality Desc:
     *   保存购买
     * @param purchaseRecord :
     * @return
     * @author Jiachen<zhangjc1999 @ gmail.com>
     * @date   2020/10/30 9:16 下午
     */
    public Boolean save(PurchaseDto purchaseRecord);

    /**
     * Functionality Desc:
     *   根据ID删除购买记录
     * @param id :
     * @return
     * @author Jiachen<zhangjc1999 @ gmail.com>
     * @date   2020/10/30 9:17 下午
     */
    public Boolean deleteById(long id);

    /**
     * Functionality Desc:
     *   根据 名称 和 类型 查找购买记录
     * @param name: 名称
     * @param type: 类型
     * @return
     * @author Jiachen<zhangjc1999 @ gmail.com>
     * @date   2020/10/30 9:17 下午
     */
    public List<Purchase>findByNameAndAndType(String name, int type);

    /**
     * Functionality Desc:
     *   根据 ID 查找购买记录
     * @param id :
     * @return
     * @author Jiachen<zhangjc1999 @ gmail.com>
     * @date   2020/10/30 9:18 下午
     */
    public PurchaseDto findById(long id);

    List<PurchaseDto> listAllRecords();
}
