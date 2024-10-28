package com.example.springproject.repository;

import com.example.springproject.domain.Purchase;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import java.util.List;

public interface PurchaseRecordRepository extends JpaRepository<Purchase, Long> {

    @Query("select p from Purchase p where p.username=?1 and p.type=?2")
    List<Purchase> findByNameAndAndType(String username, int type);



}
