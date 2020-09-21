#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVeretxArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
	vb.Bind();
    // list of all elements in m_Elements vector
    const auto& elements = layout.GetElements();

    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        // pushes data to the target buffer
        GLCall(glEnableVertexAttribArray(i));
        // formatting of data in current buffer
        GLCall(glVertexAttribPointer(i, element.count, element.type, 
            element.normalized, layout.GetStride(), (const void*)offset));

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}