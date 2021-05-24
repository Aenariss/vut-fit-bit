/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>
#include <memory>
#include <map>
#include <vector>

GPU::GPU(){
  /// \todo Zde můžete alokovat/inicializovat potřebné proměnné grafické karty
}
GPU::~GPU(){
  /// \todo Zde můžete dealokovat/deinicializovat grafickou kartu
  buffer_map.clear();
  vertex_map.clear();
  program_map.clear();
}

/// Buffers

BufferID GPU::createBuffer(uint64_t size) {
  /// \todo Tato funkce by měla na grafické kartě vytvořit buffer dat.<br>
  /// Velikost bufferu je v parameteru size (v bajtech).<br>
  /// Funkce by měla vrátit unikátní identifikátor identifikátor bufferu.<br>
  /// Na grafické kartě by mělo být možné alkovat libovolné množství bufferů o libovolné velikosti.<br>
    auto local_data = std::vector<uint8_t>(size);
    auto unique = std::make_unique<BufferStruct>();
    unique->data = local_data;
    BufferID unique_id = reinterpret_cast<BufferID>(unique.get());
    buffer_map.emplace(unique_id, std::move(unique));
    return unique_id;
}
void GPU::deleteBuffer(BufferID buffer) {
  /// \todo Tato funkce uvolní buffer na grafické kartě.
  /// Buffer pro smazání je vybrán identifikátorem v parameteru "buffer".
  /// Po uvolnění bufferu je identifikátor volný a může být znovu použit při vytvoření nového bufferu.
    auto buff = buffer_map.find(buffer);
    if (buff != buffer_map.end()) {
        buffer_map.erase(buffer);
    }
}
void GPU::setBufferData(BufferID buffer, uint64_t offset, uint64_t size, void const* data) {
  /// \todo Tato funkce nakopíruje data z cpu na "gpu".<br>
  /// Data by měla být nakopírována do bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje, kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) kam se data nakopírují.<br>
  /// Parametr data obsahuje ukazatel na data na cpu pro kopírování.<br>
    auto dest_buff = buffer_map.find(buffer);
    if (dest_buff != buffer_map.end()) {
        std::copy((uint8_t*)data, (uint8_t*)data + size, (dest_buff->second->data.begin() + offset));
    }
}
void GPU::getBufferData(BufferID buffer,
                        uint64_t offset,
                        uint64_t size,
                        void*    data)
{
  /// \todo Tato funkce vykopíruje data z "gpu" na cpu.
  /// Data by měla být vykopírována z bufferu vybraného parametrem "buffer".<br>
  /// Parametr size určuje kolik dat (v bajtech) se překopíruje.<br>
  /// Parametr offset určuje místo v bufferu (posun v bajtech) odkud se začne kopírovat.<br>
  /// Parametr data obsahuje ukazatel, kam se data nakopírují.<br>
    auto dest_buff = buffer_map.find(buffer);
    if (dest_buff != buffer_map.end()) {
        auto from = dest_buff->second->data.begin() + offset;
        std::copy(from, from+size , (uint8_t*)data);
    }
}
bool GPU::isBuffer(BufferID buffer) { 
  /// \todo Tato funkce by měla vrátit true pokud buffer je identifikátor existující bufferu.<br>
  /// Tato funkce by měla vrátit false, pokud buffer není identifikátor existujícího bufferu. (nebo bufferu, který byl smazán).<br>
  /// Pro emptyId vrací false.<br>
    auto test = buffer_map.find(buffer);
    if (test == buffer_map.end()) {
        return false;
    }
    return true;
}

///VertexPullers

ObjectID GPU::createVertexPuller     (){
  /// \todo Tato funkce vytvoří novou práznou tabulku s nastavením pro vertex puller.<br>
  /// Funkce by měla vrátit identifikátor nové tabulky.
  /// Prázdná tabulka s nastavením neobsahuje indexování a všechny čtecí hlavy jsou vypnuté.
    auto vertex = std::make_unique<VertexPullerStruct>();
    ObjectID unique_id = reinterpret_cast<ObjectID>(vertex.get());
    vertex_map.emplace(unique_id, std::move(vertex));
    return unique_id;
}
void     GPU::deleteVertexPuller     (VertexPullerID vao){
  /// \todo Tato funkce by měla odstranit tabulku s nastavení pro vertex puller.<br>
  /// Parameter "vao" obsahuje identifikátor tabulky s nastavením.<br>
  /// Po uvolnění nastavení je identifiktátor volný a může být znovu použit.<br>
    auto vertex = vertex_map.find(vao);
    if (vertex != vertex_map.end()) {
        vertex_map.erase(vao);
    }
}
void     GPU::setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer){
  /// \todo Tato funkce nastaví jednu čtecí hlavu vertex pulleru.<br>
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "head" vybírá čtecí hlavu vybraného vertex pulleru.<br>
  /// Parametr "type" nastaví typ atributu, který čtecí hlava čte. Tímto se vybere kolik dat v bajtech se přečte.<br>
  /// Parametr "stride" nastaví krok čtecí hlavy.<br>
  /// Parametr "offset" nastaví počáteční pozici čtecí hlavy.<br>
  /// Parametr "buffer" vybere buffer, ze kterého bude čtecí hlava číst.<br>
    auto vertex = vertex_map.find(vao);
    if (vertex != vertex_map.end()) {
       auto vertex_struct = vertex->second.get();
       vertex_struct->head[head].attribute_type = type;
       vertex_struct->head[head].stride = stride;
       vertex_struct->head[head].offset = offset;
       vertex_struct->head[head].buffer = buffer;
    }
}
void     GPU::setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer){
  /// \todo Tato funkce nastaví indexování vertex pulleru.
  /// Parametr "vao" vybírá tabulku s nastavením.<br>
  /// Parametr "type" volí typ indexu, který je uložený v bufferu.<br>
  /// Parametr "buffer" volí buffer, ve kterém jsou uloženy indexy.<br>
    auto vertex = vertex_map.find(vao);
    if (vertex != vertex_map.end()) {
        auto vertex_struct = vertex->second.get();
        vertex_struct->index.index_type = type;
        vertex_struct->index.buffer = buffer;
    }
}
void     GPU::enableVertexPullerHead (VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce povolí čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava povolena, hodnoty z bufferu se budou kopírovat do atributu vrcholů vertex shaderu.<br>
  /// Parametr "vao" volí tabulku s nastavením vertex pulleru (vybírá vertex puller).<br>
  /// Parametr "head" volí čtecí hlavu.<br>
    auto vertex = vertex_map.find(vao);
    if (vertex != vertex_map.end()) {
        auto vertex_struct = vertex->second.get();
        vertex_struct->head[head].enabled = true;
    }
}
void     GPU::disableVertexPullerHead(VertexPullerID vao,uint32_t head){
  /// \todo Tato funkce zakáže čtecí hlavu daného vertex pulleru.<br>
  /// Pokud je čtecí hlava zakázána, hodnoty z bufferu se nebudou kopírovat do atributu vrcholu.<br>
  /// Parametry "vao" a "head" vybírají vertex puller a čtecí hlavu.<br>
    auto vertex = vertex_map.find(vao);
    if (vertex != vertex_map.end()) {
        auto vertex_struct = vertex->second.get();
        vertex_struct->head[head].enabled = false;
    }
}
void     GPU::bindVertexPuller       (VertexPullerID vao){
  /// \todo Tato funkce aktivuje nastavení vertex pulleru.<br>
  /// Pokud je daný vertex puller aktivován, atributy z bufferů jsou vybírány na základě jeho nastavení.<br>
    auto vertex = vertex_map.find(vao);
    if (vertex != vertex_map.end()) {
        current = vertex->second.get();
    }

}
void     GPU::unbindVertexPuller     (){
  /// \todo Tato funkce deaktivuje vertex puller.
  /// To většinou znamená, že se vybere neexistující "emptyID" vertex puller.
    current = nullptr;
}
bool     GPU::isVertexPuller         (VertexPullerID vao){
  /// \todo Tato funkce otestuje, zda daný vertex puller existuje.
  /// Pokud ano, funkce vrací true.
    auto test = vertex_map.find(vao);
    if (test == vertex_map.end()) {
        return false;
    }
    return true;
}

///Programs

ProgramID        GPU::createProgram         (){
  /// \todo Tato funkce by měla vytvořit nový shader program.<br>
  /// Funkce vrací unikátní identifikátor nového proramu.<br>
  /// Program je seznam nastavení, které obsahuje: ukazatel na vertex a fragment shader.<br>
  /// Dále obsahuje uniformní proměnné a typ výstupních vertex attributů z vertex shaderu, které jsou použity pro interpolaci do fragment atributů.<br>
    auto program = std::make_unique<ProgramStruct>();
    ProgramID unique_id = reinterpret_cast<ProgramID>(program.get());
    program_map.emplace(unique_id, std::move(program));
    return unique_id;
}
void             GPU::deleteProgram         (ProgramID prg){
  /// \todo Tato funkce by měla smazat vybraný shader program.<br>
  /// Funkce smaže nastavení shader programu.<br>
  /// Identifikátor programu se stane volným a může být znovu využit.<br>
    auto program = program_map.find(prg);
    if (program != program_map.end()) {
        program_map.erase(prg);
    }
}
void             GPU::attachShaders         (ProgramID prg,VertexShader vs,FragmentShader fs){
  /// \todo Tato funkce by měla připojít k vybranému shader programu vertex a fragment shader.
    auto program = program_map.find(prg);
    if (program != program_map.end()) {
        auto program_struct = program->second.get();
        program_struct->fragment = fs;
        program_struct->vertex = vs;
    }
}
void             GPU::setVS2FSType          (ProgramID prg,uint32_t attrib,AttributeType type){
  /// \todo tato funkce by měla zvolit typ vertex atributu, který je posílán z vertex shaderu do fragment shaderu.<br>
  /// V průběhu rasterizace vznikají fragment.<br>
  /// Fragment obsahují fragment atributy.<br>
  /// Tyto atributy obsahují interpolované hodnoty vertex atributů.<br>
  /// Tato funkce vybere jakého typu jsou tyto interpolované atributy.<br>
  /// Bez jakéhokoliv nastavení jsou atributy prázdne AttributeType::EMPTY<br>
    auto program = program_map.find(prg);
    if (program != program_map.end()) {
        auto program_struct = program->second.get();
        program_struct->attribute[attrib] = type;
        
    }
}
void             GPU::useProgram            (ProgramID prg){
  /// \todo tato funkce by měla vybrat aktivní shader program.
    auto program = program_map.find(prg);
    if (program != program_map.end()) {
        used = program->second.get();
    }
}
bool             GPU::isProgram             (ProgramID prg){
  /// \todo tato funkce by měla zjistit, zda daný program existuje.<br>
  /// Funkce vráti true, pokud program existuje.<br>
    auto program = program_map.find(prg);
    if (program == program_map.end()) {
        return false;
    }
    return true;
}
void             GPU::programUniform1f      (ProgramID prg,uint32_t uniformId,float     const&d){
  /// \todo tato funkce by měla nastavit uniformní proměnnou shader programu.<br>
  /// Parametr "prg" vybírá shader program.<br>
  /// Parametr "uniformId" vybírá uniformní proměnnou. Maximální počet uniformních proměnných je uložen v programné \link maxUniforms \endlink.<br>
  /// Parametr "d" obsahuje data (1 float).<br>
    auto program = program_map.find(prg);
    if (program != program_map.end()) {
        auto program_struct = program->second.get();
        program_struct->uniform.uniform[uniformId].v1 = d;
    }
}
void             GPU::programUniform2f      (ProgramID prg,uint32_t uniformId,glm::vec2 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 2 floaty.
    auto program = program_map.find(prg);
    if (program != program_map.end()) {
        auto program_struct = program->second.get();
        program_struct->uniform.uniform[uniformId].v2 = d;
    }
}
void             GPU::programUniform3f      (ProgramID prg,uint32_t uniformId,glm::vec3 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 3 floaty.
    auto program = program_map.find(prg);
    if (program != program_map.end()) {
        auto program_struct = program->second.get();
        program_struct->uniform.uniform[uniformId].v3 = d;
    }
}
void             GPU::programUniform4f      (ProgramID prg,uint32_t uniformId,glm::vec4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává 4 floaty.
    auto program = program_map.find(prg);
    if (program != program_map.end()) {
        auto program_struct = program->second.get();
        program_struct->uniform.uniform[uniformId].v4 = d;
    }
}
void             GPU::programUniformMatrix4f(ProgramID prg,uint32_t uniformId,glm::mat4 const&d){
  /// \todo tato funkce dělá obdobnou věc jako funkce programUniform1f.<br>
  /// Místo 1 floatu nahrává matici 4x4 (16 floatů).
    auto program = program_map.find(prg);
    if (program != program_map.end()) {
        auto program_struct = program->second.get();
        program_struct->uniform.uniform[uniformId].m4= d;
    }
}

//FrameBuffer

void GPU::createFramebuffer      (uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla alokovat framebuffer od daném rozlišení.<br>w
  /// Framebuffer se skládá z barevného a hloukového bufferu.<br>
  /// Buffery obsahují width x height pixelů.<br>
  /// Barevný pixel je složen z 4 x uint8_t hodnot - to reprezentuje RGBA barvu.<br>
  /// Hloubkový pixel obsahuje 1 x float - to reprezentuje hloubku.<br>
  /// Nultý pixel framebufferu je vlevo dole.<br>
    frame_height = height;
    frame_width = width;
    frame_size = width * height;
    depth.resize(frame_size);
    color.resize(frame_size*4);

}
void GPU::deleteFramebuffer      (){
  /// \todo tato funkce by měla dealokovat framebuffer.
    frame_height = 0;
    frame_width = 0;
    frame_size = 0;
    depth.resize(0);
    color.resize(0);
}
void     GPU::resizeFramebuffer(uint32_t width,uint32_t height){
  /// \todo Tato funkce by měla změnit velikost framebuffer.
    frame_height = height;
    frame_width = width;
    frame_size = width * height;
    depth.resize(frame_size);
    color.resize(frame_size*4);

}
uint8_t* GPU::getFramebufferColor  (){
  /// \todo Tato funkce by měla vrátit ukazatel na začátek barevného bufferu.<br>
   auto it = color.begin();
    if (it != color.end()) {
        uint8_t* begin = &(*it);
        return begin;
    }
    return nullptr;
}
float* GPU::getFramebufferDepth() {
    /// \todo tato funkce by mla vrátit ukazatel na začátek hloubkového bufferu.<br>
    std::vector<float>::iterator it = depth.begin();
    if (it != depth.end()) {
        float* begin = &(*it);
        return begin;
    }
    return nullptr;
}
uint32_t GPU::getFramebufferWidth (){
  /// \todo Tato funkce by měla vrátit šířku framebufferu.
    return frame_width;
}
uint32_t GPU::getFramebufferHeight() {
    /// \todo Tato funkce by měla vrátit výšku framebufferu.
    return frame_height;
}

//DrawingFunctions

void            GPU::clear                 (float r,float g,float b,float a){
  /// \todo Tato funkce by měla vyčistit framebuffer.<br>
  /// Barevný buffer vyčistí na barvu podle parametrů r g b a (0 - nulová intenzita, 1 a větší - maximální intenzita).<br>
  /// (0,0,0) - černá barva, (1,1,1) - bílá barva.<br>
  /// Hloubkový buffer nastaví na takovou hodnotu, která umožní rasterizaci trojúhelníka, který leží v rámci pohledového tělesa.<br>
  /// Hloubka by měla být tedy větší než maximální hloubka v NDC (normalized device coordinates).<br>


}
void            GPU::drawTriangles         (uint32_t  nofVertices){
  /// \todo Tato funkce vykreslí trojúhelníky podle daného nastavení.<br>
  /// Vrcholy se budou vybírat podle nastavení z aktivního vertex pulleru (pomocí bindVertexPuller).<br>
  /// Vertex shader a fragment shader se zvolí podle aktivního shader programu (pomocí useProgram).<br>
  /// Parametr "nofVertices" obsahuje počet vrcholů, který by se měl vykreslit (3 pro jeden trojúhelník).<br>
    auto use_vertex = current;
    auto use_program = used;
    bool Indexing = isBuffer(use_vertex->index.buffer);
    auto in_vertex = new InVertex();
    auto out_vertex = new OutVertex(); 

    uint32_t counter = 1;
    uint32_t vertexID = 0;
    while (counter <= nofVertices) {
        auto uniforms = use_program->uniform;
        //Indexing false == increment and use it as an ID
        if (Indexing == false) {
            in_vertex->gl_VertexID = vertexID++;
        }
        else {
            //Indexing true == the ID is set into the buffer, so get it from it
            getBufferData(use_vertex->index.buffer, vertexID * (uint64_t)use_vertex->index.index_type, (uint64_t)use_vertex->index.index_type, &in_vertex->gl_VertexID);
            vertexID++;
        }
        //set the VS attributes
        for (int i = 0; i < maxAttributes; i++) {

            getBufferData(use_vertex->head[i].buffer, use_vertex->head[i].offset + use_vertex->head[i].stride * in_vertex->gl_VertexID,
            (uint64_t)use_vertex->head[i].attribute_type * 4, &in_vertex->attributes[i]);
        }

        use_program->vertex(*out_vertex, *in_vertex, uniforms);
        counter++;

        
        
    }
}

