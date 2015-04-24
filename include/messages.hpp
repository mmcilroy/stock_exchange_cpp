#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

class header
{
private:
    char* buf_;

public:
    header( void* buf ) : buf_( (char*)buf ) {
        ;
    }

    static constexpr size_t size() {
        return 4;
    }

    static constexpr size_t id() {
        return 0;
    }

    const char* buffer() {
        return buf_;
    }

    void length( uint16_t v ) {
        *(uint16_t*)(buf_+0) = v;
    }

    uint16_t length() const {
        return *(uint16_t*)(buf_+0);
    }

    void type( uint16_t v ) {
        *(uint16_t*)(buf_+2) = v;
    }

    uint16_t type() const {
        return *(uint16_t*)(buf_+2);
    }

};

class login
{
private:
    char* buf_;

public:
    login( void* buf ) : buf_( (char*)buf ) {
        ;
    }

    static constexpr size_t size() {
        return 4;
    }

    static constexpr size_t id() {
        return 1;
    }

    const char* buffer() {
        return buf_;
    }

    void user_id( uint32_t v ) {
        *(uint32_t*)(buf_+0) = v;
    }

    uint32_t user_id() const {
        return *(uint32_t*)(buf_+0);
    }

};

class order_parameters
{
private:
    char* buf_;

public:
    order_parameters( void* buf ) : buf_( (char*)buf ) {
        ;
    }

    static constexpr size_t size() {
        return 12;
    }

    static constexpr size_t id() {
        return 2;
    }

    const char* buffer() {
        return buf_;
    }

    void type( uint16_t v ) {
        *(uint16_t*)(buf_+0) = v;
    }

    uint16_t type() const {
        return *(uint16_t*)(buf_+0);
    }

    void side( uint16_t v ) {
        *(uint16_t*)(buf_+2) = v;
    }

    uint16_t side() const {
        return *(uint16_t*)(buf_+2);
    }

    void quantity( uint32_t v ) {
        *(uint32_t*)(buf_+4) = v;
    }

    uint32_t quantity() const {
        return *(uint32_t*)(buf_+4);
    }

    void price( uint32_t v ) {
        *(uint32_t*)(buf_+8) = v;
    }

    uint32_t price() const {
        return *(uint32_t*)(buf_+8);
    }

};

class place_order
{
private:
    char* buf_;

public:
    place_order( void* buf ) : buf_( (char*)buf ) {
        ;
    }

    static constexpr size_t size() {
        return 20;
    }

    static constexpr size_t id() {
        return 3;
    }

    const char* buffer() {
        return buf_;
    }

    void user_id( uint32_t v ) {
        *(uint32_t*)(buf_+0) = v;
    }

    uint32_t user_id() const {
        return *(uint32_t*)(buf_+0);
    }

    void transaction_id( uint32_t v ) {
        *(uint32_t*)(buf_+4) = v;
    }

    uint32_t transaction_id() const {
        return *(uint32_t*)(buf_+4);
    }

    order_parameters parameters() {
        return order_parameters(buf_+8);
    }

};

class pull_order
{
private:
    char* buf_;

public:
    pull_order( void* buf ) : buf_( (char*)buf ) {
        ;
    }

    static constexpr size_t size() {
        return 8;
    }

    static constexpr size_t id() {
        return 4;
    }

    const char* buffer() {
        return buf_;
    }

    void user_id( uint32_t v ) {
        *(uint32_t*)(buf_+0) = v;
    }

    uint32_t user_id() const {
        return *(uint32_t*)(buf_+0);
    }

    void transaction_id( uint32_t v ) {
        *(uint32_t*)(buf_+4) = v;
    }

    uint32_t transaction_id() const {
        return *(uint32_t*)(buf_+4);
    }

};

class order_rejected
{
private:
    char* buf_;

public:
    order_rejected( void* buf ) : buf_( (char*)buf ) {
        ;
    }

    static constexpr size_t size() {
        return 20;
    }

    static constexpr size_t id() {
        return 5;
    }

    const char* buffer() {
        return buf_;
    }

    void user_id( uint32_t v ) {
        *(uint32_t*)(buf_+0) = v;
    }

    uint32_t user_id() const {
        return *(uint32_t*)(buf_+0);
    }

    void transaction_id( uint32_t v ) {
        *(uint32_t*)(buf_+4) = v;
    }

    uint32_t transaction_id() const {
        return *(uint32_t*)(buf_+4);
    }

    order_parameters parameters() {
        return order_parameters(buf_+8);
    }

};

class order_placed
{
private:
    char* buf_;

public:
    order_placed( void* buf ) : buf_( (char*)buf ) {
        ;
    }

    static constexpr size_t size() {
        return 20;
    }

    static constexpr size_t id() {
        return 6;
    }

    const char* buffer() {
        return buf_;
    }

    void user_id( uint32_t v ) {
        *(uint32_t*)(buf_+0) = v;
    }

    uint32_t user_id() const {
        return *(uint32_t*)(buf_+0);
    }

    void transaction_id( uint32_t v ) {
        *(uint32_t*)(buf_+4) = v;
    }

    uint32_t transaction_id() const {
        return *(uint32_t*)(buf_+4);
    }

    order_parameters parameters() {
        return order_parameters(buf_+8);
    }

};

class order_pulled
{
private:
    char* buf_;

public:
    order_pulled( void* buf ) : buf_( (char*)buf ) {
        ;
    }

    static constexpr size_t size() {
        return 24;
    }

    static constexpr size_t id() {
        return 7;
    }

    const char* buffer() {
        return buf_;
    }

    void user_id( uint32_t v ) {
        *(uint32_t*)(buf_+0) = v;
    }

    uint32_t user_id() const {
        return *(uint32_t*)(buf_+0);
    }

    void transaction_id( uint32_t v ) {
        *(uint32_t*)(buf_+4) = v;
    }

    uint32_t transaction_id() const {
        return *(uint32_t*)(buf_+4);
    }

    void leaves_quantity( uint32_t v ) {
        *(uint32_t*)(buf_+8) = v;
    }

    uint32_t leaves_quantity() const {
        return *(uint32_t*)(buf_+8);
    }

    order_parameters parameters() {
        return order_parameters(buf_+12);
    }

};

class order_executed
{
private:
    char* buf_;

public:
    order_executed( void* buf ) : buf_( (char*)buf ) {
        ;
    }

    static constexpr size_t size() {
        return 32;
    }

    static constexpr size_t id() {
        return 8;
    }

    const char* buffer() {
        return buf_;
    }

    void user_id( uint32_t v ) {
        *(uint32_t*)(buf_+0) = v;
    }

    uint32_t user_id() const {
        return *(uint32_t*)(buf_+0);
    }

    void transaction_id( uint32_t v ) {
        *(uint32_t*)(buf_+4) = v;
    }

    uint32_t transaction_id() const {
        return *(uint32_t*)(buf_+4);
    }

    void exec_price( uint32_t v ) {
        *(uint32_t*)(buf_+8) = v;
    }

    uint32_t exec_price() const {
        return *(uint32_t*)(buf_+8);
    }

    void exec_quantity( uint32_t v ) {
        *(uint32_t*)(buf_+12) = v;
    }

    uint32_t exec_quantity() const {
        return *(uint32_t*)(buf_+12);
    }

    void leaves_quantity( uint32_t v ) {
        *(uint32_t*)(buf_+16) = v;
    }

    uint32_t leaves_quantity() const {
        return *(uint32_t*)(buf_+16);
    }

    order_parameters parameters() {
        return order_parameters(buf_+20);
    }

};

std::ostream& operator<<( std::ostream& o, header v );
std::ostream& operator<<( std::ostream& o, login v );
std::ostream& operator<<( std::ostream& o, order_parameters v );
std::ostream& operator<<( std::ostream& o, place_order v );
std::ostream& operator<<( std::ostream& o, pull_order v );
std::ostream& operator<<( std::ostream& o, order_rejected v );
std::ostream& operator<<( std::ostream& o, order_placed v );
std::ostream& operator<<( std::ostream& o, order_pulled v );
std::ostream& operator<<( std::ostream& o, order_executed v );
