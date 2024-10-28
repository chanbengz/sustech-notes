package com.example.springproject.dto;

import com.example.springproject.domain.UserInfo;
import io.swagger.annotations.ApiModel;
import io.swagger.annotations.ApiModelProperty;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.hibernate.validator.constraints.Length;

import javax.validation.constraints.NotBlank;

@Data
@NoArgsConstructor
@AllArgsConstructor
@Builder
@ApiModel(description = "user form")
public class UserForm {

    @NotBlank(message = "Username shouldn't be null")
    @ApiModelProperty(value = "username")
    private String username;

    @Length(min = 6, message = "Password need at least 6 bits")
    @ApiModelProperty(value = "password")
    private String password;

    @NotBlank(message = "Confirm password shouldn't be null")
    @ApiModelProperty(value = "confirmPassword")
    private String confirmPassword;

    public boolean checkPasswordEquals(){
        return this.password.equals(this.confirmPassword);
    }

    public UserInfo convertToUser(){
        return new UserInfo(username,password);
    }
}
