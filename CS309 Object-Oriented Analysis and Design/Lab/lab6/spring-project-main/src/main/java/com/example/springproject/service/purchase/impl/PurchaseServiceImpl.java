package com.example.springproject.service.purchase.impl;

import com.example.springproject.dto.PurchaseDto;
import com.example.springproject.repository.PurchaseRecordRepository;
import com.example.springproject.domain.Purchase;
import com.example.springproject.service.purchase.PurchaseService;
import java.util.stream.Collectors;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class PurchaseServiceImpl implements PurchaseService {

    private final PurchaseRecordRepository purchaseRecordRepository;

    @Autowired
    public PurchaseServiceImpl(PurchaseRecordRepository purchaseRecordRepository) {
        this.purchaseRecordRepository = purchaseRecordRepository;
    }

    @Override
    public List<Purchase> findAll() {
        return purchaseRecordRepository.findAll();
    }

    @Override
    public Boolean save(PurchaseDto request) {
        Purchase record = convertPurchaseRequest(request);
        purchaseRecordRepository.save(record);
        return Boolean.TRUE;
    }

    @Override
    public Boolean deleteById(long id) {
        purchaseRecordRepository.deleteById(id);
        return Boolean.TRUE;
    }

    @Override
    public List<Purchase> findByNameAndAndType(String name, int type) {
        return purchaseRecordRepository.findByNameAndAndType(name, type);
    }

    @Override
    public PurchaseDto findById(long id) {
        Purchase record = purchaseRecordRepository.findById(id).get();
        return convertPurchaseRecord(record);
    }


    @Override
    public List<PurchaseDto> listAllRecords() {
        List<Purchase> recordList = purchaseRecordRepository.findAll();
        return recordList.stream().map(this::convertPurchaseRecord).collect(Collectors.toList());
    }

    private PurchaseDto convertPurchaseRecord(Purchase record) {
        return PurchaseDto.builder()
            .id(record.getId())
            .username(record.getUsername())
            .date(record.getDate())
            .money(record.getMoney())
            .description(record.getDescription())
            .type(record.getType())
            .build();
    }

    private Purchase convertPurchaseRequest(PurchaseDto request) {
        return Purchase.builder()
            .id(request.getId())
            .username(request.getUsername())
            .date(request.getDate())
            .money(request.getMoney())
            .description(request.getDescription())
            .type(request.getType())
            .build();
    }

}
