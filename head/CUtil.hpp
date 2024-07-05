/*
 * This file is part of the software and assets of HK ZXOUD LIMITED.
 * @copyright (c) HK ZXOUD LIMITED https://www.zxoud.com
 * Author: yushou-cell(email:2354739167@qq.com)
 * create: 20240622
 * FilePath: /zlink/head/CUtil.hpp
 * Description:
 */
#pragma once
#include "system.hpp"

struct CUtil
{
    /**
     * @description: print trace
     * @return {string} trace result
     */
    static std::string printTrace();
    /**
     * @description: construct redis server response messages
     * @param {json} &req: send to redis command as nlohmann::json format
     * @return {string} response from redis
     */
    static std::string constructResponseMsgRedis(const nlohmann::json &req);
    /**
     * @description: construct PostgreSQL server response messages
     * @param {json} &req: send to PostgreSQL command as nlohmann::json format
     * @return {response from PostgreSQL} response from PostgreSQL
     */
    static std::string constructResponseMsgPgSQL(const nlohmann::json &req);
    /**
     * @description: parse ZeroMQ messages into std::string data
     * @param {zmsg_t} *msg: raw ZeroMQ pointer
     * @param {string} &msgStr: converted value
     */
    static inline void getZMsg(zmsg_t *msg, std::string &msgStr)
    {
        char *frameContent;
        while ((frameContent = zmsg_popstr(msg)) != NULL)
        {
            // handle frame content
            msgStr.append(frameContent);
            LOG(INFO) << "Frame content: " << frameContent;
            free(frameContent);
        }
    }
};
