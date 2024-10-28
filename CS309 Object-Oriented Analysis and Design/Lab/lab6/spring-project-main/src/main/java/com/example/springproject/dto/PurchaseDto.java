package com.example.springproject.dto;

import io.swagger.annotations.ApiModel;
import io.swagger.annotations.ApiModelProperty;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
@Builder
@ApiModel
public class PurchaseDto {
    @ApiModelProperty
    private long id;
    @ApiModelProperty
    private String username;
    @ApiModelProperty
    private String date;
    @ApiModelProperty
    private double money;
    @ApiModelProperty
    private int type;
    @ApiModelProperty
    private String description;
}
