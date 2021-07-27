/*!
 * @file
 * @brief This file contains class that represents graphic card.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#pragma once
#include <stddef.h>
#include <string.h>
#include <memory>
#include <map>
#include <student/fwd.hpp>
#include <vector>


/**
 * @brief This class represent software GPU
 */
class GPU{
  public:
    GPU();
    virtual ~GPU();

    //buffer object commands
    BufferID  createBuffer           (uint64_t size);
    void      deleteBuffer           (BufferID buffer);
    void      setBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void const* data);
    void      getBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void      * data);
    bool      isBuffer               (BufferID buffer);

    //vertex array object commands (vertex puller)
    ObjectID  createVertexPuller     ();
    void      deleteVertexPuller     (VertexPullerID vao);
    void      setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer);
    void      setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer);
    void      enableVertexPullerHead (VertexPullerID vao,uint32_t head);
    void      disableVertexPullerHead(VertexPullerID vao,uint32_t head);
    void      bindVertexPuller       (VertexPullerID vao);
    void      unbindVertexPuller     ();
    bool      isVertexPuller         (VertexPullerID vao);

    //program object commands
    ProgramID createProgram          ();
    void      deleteProgram          (ProgramID prg);
    void      attachShaders          (ProgramID prg,VertexShader vs,FragmentShader fs);
    void      setVS2FSType           (ProgramID prg,uint32_t attrib,AttributeType type);
    void      useProgram             (ProgramID prg);
    bool      isProgram              (ProgramID prg);
    void      programUniform1f       (ProgramID prg,uint32_t uniformId,float     const&d);
    void      programUniform2f       (ProgramID prg,uint32_t uniformId,glm::vec2 const&d);
    void      programUniform3f       (ProgramID prg,uint32_t uniformId,glm::vec3 const&d);
    void      programUniform4f       (ProgramID prg,uint32_t uniformId,glm::vec4 const&d);
    void      programUniformMatrix4f (ProgramID prg,uint32_t uniformId,glm::mat4 const&d);

    //framebuffer functions
    void      createFramebuffer      (uint32_t width,uint32_t height);
    void      deleteFramebuffer      ();
    void      resizeFramebuffer      (uint32_t width,uint32_t height);
    uint8_t*  getFramebufferColor    ();
    float*    getFramebufferDepth    ();
    uint32_t  getFramebufferWidth    ();
    uint32_t  getFramebufferHeight   ();

    //execution commands
    void      clear                  (float r,float g,float b,float a);
    void      drawTriangles          (uint32_t  nofVertices);

    /// \addtogroup gpu_init 00. proměnné, inicializace / deinicializace grafické karty
    /// @{
    /// \todo zde si můžete vytvořit proměnné grafické karty (buffery, programy, ...)
    /// @}
    struct Indexing {
        BufferID buffer;
        IndexType index_type;
    };
    struct HeadX {
        BufferID buffer;
        AttributeType attribute_type;
        uint64_t stride;
        uint64_t offset;
        bool enabled = false;
    };

    struct VertexPullerStruct {
        Indexing index;
        HeadX head[maxAttributes];
    };

    struct ProgramStruct {
        FragmentShader fragment;
        VertexShader vertex;
        Uniforms uniform;
        AttributeType attribute[maxAttributes];
    };

    struct BufferStruct {
        std::vector<uint8_t> data;
    };

    //Buffer
    std::map<BufferID, std::unique_ptr<BufferStruct> > buffer_map;
    //VertexPuller
    std::map<VertexPullerID, std::unique_ptr<VertexPullerStruct> > vertex_map;
    VertexPullerStruct* current = nullptr;
    //Program
    std::map<ProgramID, std::unique_ptr<ProgramStruct> > program_map;
    ProgramStruct* used = nullptr;
    //FrameBuffer
    uint32_t frame_height;
    uint32_t frame_width;
    uint64_t frame_size;
    std::vector<float> depth;
    std::vector<uint8_t> color;

};