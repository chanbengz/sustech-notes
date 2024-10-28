package com.example.springproject.controller;

import com.example.springproject.config.MyException;
import com.example.springproject.dto.PurchaseDto;
import com.example.springproject.service.purchase.PurchaseService;
import java.util.Objects;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping(path = "/api/purchase")
public class PurchaseController {

    private final PurchaseService purchaseService;

    @Autowired
    public PurchaseController(PurchaseService purchaseService) {
        this.purchaseService = purchaseService;
    }

    @GetMapping("/v1/records")
    public List<PurchaseDto> listAllRecords(){
        return purchaseService.listAllRecords();
    }

    @PostMapping("/v1/record")
    public Boolean saveNew(@RequestBody PurchaseDto request){
        request.setId(-1);
        return purchaseService.save(request);
    }

    @PutMapping("/v1/record/{id}")
    public PurchaseDto updateRecord(@PathVariable long id, @RequestBody PurchaseDto request){
        request.setId(id);
        return purchaseService.findById(id);
    }

    @GetMapping("/v1/record/{id}")
    public PurchaseDto findPurchaseRecord(@PathVariable long id){
        return purchaseService.findById(id);
    }

    @DeleteMapping("/v1/record/{id}")
    public Boolean deletePurchaseRecord(@PathVariable long id){
        return purchaseService.deleteById(id);
    }


}
